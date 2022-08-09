#include "Listener.hpp"
#include "Request.hpp"
#include "./config/RequestConfig.hpp"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include "header.hpp"

Listener::Listener(Config& config) : _config(config)
{}

Listener::Listener(Listener const &other) : _config(other._config)
{
	_listen_fd = other._listen_fd;
	_addrlen = other._addrlen;
	_listen = other._listen;
	_address = other._address;
	_sockets = other._sockets;
	_written = other._written;
}

int Listener::init(t_listen listen_data, int queue)
{
	_listen = listen_data;

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_listen_fd == 0){
		std::cerr << "error: socket creation\n";
		return -1;
	}

	int yes = 1;
	setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(listen_data.host);
	_address.sin_port = htons(listen_data.port);

	memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));

	if(bind(_listen_fd, (sockaddr *)&_address, sizeof(_address)) < 0){
		std::cerr << "error: bind\n";
		return -1;
	}
	if (listen(_listen_fd, queue) < 0){
		std::cerr << "error: listen\n";
		return -1;
	}

	_addrlen = sizeof(_address);
	return 0;
}


int Listener::_decodeChunks(std::string &request)
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


int Listener::_process(std::string &request, content_type type)
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
	RequestConfig conf = _config.getConfigForRequest(_listen, req);
	ServResponse.SetDefaultErrorPages(conf.getErrorPages());


	if(!conf.getAllowedMethods().count(conf.getMethod()))
		conf.setCode(405);
	if(conf.getBody().size() > conf.getClientBodyBufferSize())
		conf.setCode(413);
	std::set<std::string> methods = conf.getAllowedMethods();

	std::cout << "reqMethod: " << conf.getMethod() << '\n';
	std::cout << "respCode: " << conf.getCode() << '\n';
	std::cout << "contentLoc: " << conf.getContentLocation() << '\n';
	std::cout << "path: " << conf.getPath() << '\n';
	std::cout << "buffSize: " << conf.getClientBodyBufferSize() << '\n';
	std::cout << "methods: " << '\n';
	for(std::set<std::string>::const_iterator it = methods.begin();
		it != methods.end(); ++it)
		std::cout << '\t' << *it << '\n';
	std::string body = conf.getBody();
	if(body.size() < 300)
		std::cout << "reqBody: " << body << '\n';

	ServResponse.SetContentType(getFileType(conf.getPath()));


	CGIResponse CGI(conf);
	CGI.SetEnvp(conf);
	if (CGI.HasSuchScript(conf.getPath()))
	{
		CGI.ExecuteCGIAndRedirect();
		CGI.MakeResponse();
		request = CGI.GetCGIResponse();
		ServResponse.SetIsCGI(true);
	}
	std::cout << "error code " << conf.getCode() << std::endl;
	if (ServResponse.IsDir(conf.getPath())) //change this?
	{
		if (conf.getAutoIndex())
		{
			ServResponse.GetDirectoryListing(conf);
			ServResponse.ShowDirectoryListing();
			request = ServResponse.GetResponse();
		}
		else
		{
			conf.setCode(403);
			ServResponse.StartThings(conf);
			request = ServResponse.GetResponse();
		}

	}
	else if (!ServResponse.GetIsCGI())
	{
		ServResponse.StartThings(conf);
		request = ServResponse.GetResponse();
	}
	std::cout << "RESPONSE FINAL SIZE:\n";
	std::cout << request.size() << std::endl;

	return 0;
}

int Listener::accept()
{
	int new_socket = ::accept(_listen_fd, (sockaddr*)&_address,
									(socklen_t*)&_addrlen);
	if(new_socket <= 0)
		std::cerr << "error: accept\n";
	else{
		std::cout << "new connection on socket " << new_socket << '\n';
		_sockets[new_socket] = "";
	}
	return new_socket;
}

int Listener::read(int socket)
{
	char buff[PACK_SIZE] = {0};
	content_type type = plain;


	int ret = ::read(socket, buff, PACK_SIZE);
	if(ret <= 0){
		close(socket);
		if(ret < 0)
			std::cerr << "error: socket " << socket << '\n';
		std::cout << "client on socket " << socket << " closed connection\n";
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

int Listener::write(int socket)
{
	char buff[PACK_SIZE] = {0};
	static bool first = true;
	if(_written.count(socket) == 0)
		_written[socket] = 0;


	size_t &written = _written[socket];
	std::string &response = _sockets[socket];
	size_t i;

	for(i = 0; i < PACK_SIZE && i + written < response.size(); ++i)
		buff[i] = response[i + written];

	int ret = ::write(socket, buff, i);

	if(ret == -1){
		std::cerr << "error: write\n";
		close(socket);
		return -1;
	}

	written += ret;
	
	if(written >= response.size()){
		_sockets.erase(socket);
		written = 0;
		return 0;
	}
	return 1;
}

void Listener::close(int socket)
{
	::close(socket);
	this->_sockets.erase(socket);
}

int Listener::getFD()
{
	return _listen_fd;
}
