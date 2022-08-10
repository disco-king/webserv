#include "Interface.hpp"
#include "../request/Request.hpp"
#include "../response/CGIResponse.hpp"
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include "../utils/utils.hpp"

Interface::Interface(Config& config) : _config(config)
{}

Interface::Interface(Interface const &other) : _config(other._config)
{
	_listen_fd = other._listen_fd;
	_addrlen = other._addrlen;
	_listen = other._listen;
	_address = other._address;
	_sockets = other._sockets;
	_written = other._written;
}

int Interface::init(t_listen listen_data, int queue)
{
	_listen = listen_data;

	std::cout << GRAY << "Establishing server on port "
		<< _listen.port << RESET << '\n';

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_listen_fd == 0){
		perror("socket");
		return -1;
	}

	int yes = 1;
	setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(listen_data.host);
	_address.sin_port = htons(listen_data.port);

	memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));

	if(bind(_listen_fd, (sockaddr *)&_address, sizeof(_address)) < 0){
		perror("bind");
		return -1;
	}
	if (listen(_listen_fd, queue) < 0){
		perror("listen");
		return -1;
	}

	_addrlen = sizeof(_address);
	return 0;
}


int Interface::_decodeChunks(std::string &request)
{
	size_t pos = request.find("\r\n\r\n");
	std::string processed = request.substr(0, pos + 4);

	size_t chunk_size = std::strtoll(&(request[pos + 4]), 0, 16);
	pos = request.find("\r\n", pos + 4);
	size_t overall_size = request.size() - pos - 7;
	while(chunk_size)
	{
		if(chunk_size > overall_size)
			break;
		processed += request.substr(pos + 2, chunk_size);
		pos += chunk_size + 4;
		chunk_size = std::strtoll(&(request[pos]), 0, 16);
		if(chunk_size == 0 && request[pos] != '0')
			return 1;
		pos = request.find("\r\n", pos);
		overall_size = request.size() - pos - 7;
	}
	if(processed.size() == processed.find("\r\n\r\n") + 4)
		return 1;
	request = processed;
	return 0;
}


void Interface::_getResponse(Response& ServResponse, RequestConfig &conf, std::string &response)
{
	ServResponse.SetContentType(getFileType(conf.getPath()));
	CGIResponse CGI(conf);
	CGI.SetEnvp(conf);
	if (CGI.IsPythonScript(conf.getPath()))
	{
		CGI.ExecuteCGIAndRedirect();
		CGI.MakeResponse();
		response = CGI.GetCGIResponse();
		ServResponse.SetIsCGI(true);
		if (CGI.GetFailed())
		{
			ServResponse.SetIsCGI(false);
			ServResponse.SetResponseCode(500);
		}
	}
	if (is_dirname(conf.getPath()) && conf.getMethod().compare("POST"))
	{
		if (conf.getAutoIndex())
		{
			ServResponse.GetDirectoryListing(conf);
			ServResponse.ShowDirectoryListing();
			response = ServResponse.GetResponse();
		}
		else
		{
			conf.setCode(403);
			ServResponse.StartThings(conf);
			response = ServResponse.GetResponse();
		}

	}
	else if (!ServResponse.GetIsCGI())
	{
		ServResponse.SetPathToFile(conf.getPath());
		ServResponse.StartThings(conf);
		response = ServResponse.GetResponse();
	}
	conf.setCode(ServResponse.GetResponseCode());
}

int Interface::_process(std::string &request, content_type type)
{
	Response ServResponse("text/html", 0, "");

	int decode_res = 0;
	if(type == chunking)
		decode_res = _decodeChunks(request);
	
	if(decode_res){
		ServResponse.MakeHTTPResponse(422);
		request = ServResponse.GetResponse();
		return 0;
	}
	Request req(request);
	req.parseRequest();

	std::cout << BLUE << req.getMethod() << ' '
		<< req.getPath() << ' ' << req.getVersion()  << RESET << '\n';

	RequestConfig conf = _config.getConfigForRequest(_listen, req);
	ServResponse.SetDefaultErrorPages(conf.getErrorPages());
	ServResponse.SetServerName(conf.getServerName());

	if(!conf.getAllowedMethods().count(conf.getMethod()))
		conf.setCode(405);
	if(conf.getBody().size() > conf.getClientBodyBufferSize())
		conf.setCode(413);

	_getResponse(ServResponse, conf, request);

	int code = (conf.getCode() == 0) ? 200 : conf.getCode();
	std::string color = code >= 400 ? RED : GREEN;
	std::cout << color << "HTTP/1.1 "
		<< code
		<< ' ' << ServResponse.getCodeRep(code)
		<< RESET << '\n';
	return 0;
}

int Interface::accept()
{
	int new_socket = ::accept(_listen_fd, (sockaddr*)&_address,
									(socklen_t*)&_addrlen);
	if(new_socket <= 0)
		perror("accept");
	else{
		std::cout << GRAY << ">>> New connection on socket "
			<< new_socket << RESET << '\n';
		_sockets[new_socket] = "";
	}
	return new_socket;
}

int Interface::read(int socket)
{
	char buff[PACK_SIZE] = {0};
	content_type type = plain;


	int ret = ::read(socket, buff, PACK_SIZE);
	if(ret <= 0){
		close(socket);
		if(ret < 0)
			perror("read");
		std::cout << GRAY << "<<< Client on socket "
			<< socket << " closed connection" << RESET << '\n';
		return -1;
	}


	std::string &request = _sockets[socket];
	for(int i = 0; i < ret; ++i)
		request.push_back(buff[i]);

	size_t head_end = request.find("\r\n\r\n");
	if(head_end == std::string::npos)
		return 1;

	size_t pos = find_string(request, "Content-Length:");
	if(pos != std::string::npos && pos < head_end){
		size_t len = std::strtoll(&(request[pos + 15]), 0, 10);
		if(request.size() < len + head_end + 4)
			return 1;
		type = length;
	}
	else {
		pos = find_string(request, "Transfer-Encoding: Chunked");
		if(pos != std::string::npos && pos < head_end){
			if(!(ends_with(request, "0\r\n\r\n") && request.size() > head_end + 4))
				return 1;
			type = chunking;
		}
	}
	_process(request, type);
	return 0;
}

int Interface::_writeFromFile(int socket, size_t head_end)
{
	static char buff[PACK_SIZE] = {0};
	static std::map<int, int> files;
	size_t i = 0;

	std::string fname = _sockets[socket].c_str() + head_end;
	if(!files.count(socket))
	{
		int fd = ::open(fname.c_str(), O_RDONLY);
		if(fd < 0){
			perror("open");
			close(socket);
			return -1;
		}
		files[socket] = fd;
		for(; i < head_end - 14; ++i)
			buff[i] = _sockets[socket][i];
	}

	int &file = files[socket];

	int res = ::read(file, buff + i, PACK_SIZE - i);
	if(res < 0){
		perror("read");
		::close(file);
		close(socket);
		return -1;
	}

	res = ::write(socket, buff, res + i);
	if(res < 0){
		perror("write");
		::close(file);
		files.erase(file);
		close(socket);
		return -1;
	}

	if(res == 0){
		::close(file);
		_sockets.erase(socket);
		files.erase(socket);
		return 0;
	}

	return 1;
}

int Interface::write(int socket)
{
	std::string &response = _sockets[socket];


	size_t header = response.find("file_abs_path:");
	if(header == response.find("\r\n\r\n") + 4)
		return _writeFromFile(socket, header + 14);

	if(!_written.count(socket))
		_written[socket] = 0;

	size_t &written = _written[socket];

	int res = ::write(socket, response.c_str() + written, PACK_SIZE);

	if(res == -1){
		perror("write");
		close(socket);
		return -1;
	}

	written += res;
	if(written >= response.size()){
		_sockets.erase(socket);
		written = 0;
		return 0;
	}
	return 1;
}


void Interface::close(int socket)
{
	::close(socket);
	this->_sockets.erase(socket);
}

int Interface::getFD()
{
	return _listen_fd;
}
