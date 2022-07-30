#include "Listener.hpp"
#include "Request.hpp"
#include "./config/RequestConfig.hpp"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>

Listener::Listener(Config& config) : _config(config)
{
	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nHello from server!";
}

Listener::Listener(Listener const &other) : _config(other._config)
{
	_listen_fd = other._listen_fd;
	_addrlen = other._addrlen;
	_listen = other._listen;
	_address = other._address;
	_response = other._response;
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
	size_t overall_size = request.size() - pos - 7;//max potential sum of chunk bodies size left
	while(chunk_size)
	{
		if(chunk_size > overall_size)
			break;

		processed += request.substr(pos + 2, chunk_size);
		pos += chunk_size + 4;
		chunk_size = std::strtoll(&(request[pos]), 0, 16);
		pos = request.find("\r\n", pos);
		overall_size = request.size() - pos - 7;
	}
	request = processed + "\r\n\r\n";
	return chunk_size;
}


int Listener::_process(std::string &request, content_type type)
{
	if(type == chunking)
		_decodeChunks(request);

	Request req(request);
	req.parseRequest();
	RequestConfig conf = _config.getConfigForRequest(_listen, req);

	std::set<std::string> methods = conf.getAllowedMethods();

	std::cout << "contentLoc: " << conf.getContentLocation() << '\n';
	std::cout << "path: " << conf.getPath() << '\n';
	std::cout << "buffSize: " << conf.getClientBodyBufferSize() << '\n';
	std::cout << "methods: " << '\n';
	for(std::set<std::string>::const_iterator it = methods.begin();
		it != methods.end(); ++it)
		std::cout << '\t' << *it << '\n';

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
	request += buff;

	size_t head_end = request.find("\r\n\r\n");
	if(head_end == std::string::npos)
		return 1;

	size_t pos = request.find("Content-Length:");
	if(pos != std::string::npos && pos < head_end){
		size_t len = std::strtoll(&(request[pos + 15]), 0, 10);
		if(request.size() < len + head_end + 4)
			return 1;
		type = length;
	}
	else {
		pos = request.find("Transfer-Encoding: Chunked");
		if(pos != std::string::npos && pos < head_end){
			if(!(ends_with(request, "0\r\n\r\n") && request.size() > head_end + 4))
				return 1;
		}
		type = chunking;
	}

	_process(request, type);

	//request return test

	// Request req(request);
	// req.parseRequest();

	// std::map<std::string, std::string> &hdrs = req.getHeaders();

	// std::cout << "METHOD: " << req.getMethod() << '\n';
	// std::cout << "PATH: " << req.getPath() << '\n';
	// std::cout << "VERSION: " << req.getVersion() << '\n';
	// std::cout << "HEADERS:\n";
	// for (std::map<std::string, std::string>::iterator i = hdrs.begin(); i != hdrs.end(); ++i)
	// 	std::cout << '<' << i->first << '>' << ": " << '[' << i->second << ']' << '\n';
	// std::cout << "BODY:\n" << req.getBody() << '\n';
	// std::cout << "CODE: " << req.getCode() << '\n';
	return 0;
}

int Listener::write(int socket)
{
	if(_written.count(socket) == 0)
		_written[socket] = 0;

	std::string to_send = _response.substr(_written[socket],PACK_SIZE);
	int ret = ::write(socket, to_send.c_str(), to_send.length());

	if(ret == -1){
		std::cerr << "error: write\n";
		close(socket);
		return -1;
	}

	size_t &written = _written[socket];
	written += ret;
	
	if(written >= _response.size()){
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
