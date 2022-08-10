#pragma once

#include <netinet/in.h>
#include <map>
#include <iostream>
#include "../utils/utils.hpp"
#include "../config/Config.hpp"
#include "../response/Response.hpp"
#include "../config/RequestConfig.hpp"

#define PACK_SIZE 65536

class Interface
{

private:
	int _listen_fd;
	int _addrlen;
	t_listen _listen;
	sockaddr_in _address;
	std::map<int, std::string> _sockets;
	std::map<int, size_t> _written;
	Config & _config;

	enum content_type{
		plain,
		chunking,
		length
	};

	int _process(std::string &request, content_type type);
	int _decodeChunks(std::string &request);
	int _writeFromFile(int socket, size_t head_end = 0);
	void _getResponse(Response& ServResponse, RequestConfig &conf, std::string &response);

public:
	Interface(Config& config);
	Interface(Interface const& other);

	int init(t_listen listen, int queue = 1000);
	int read(int socket);
	int write(int socket);
	void close(int socket);
	int accept();
	int getFD();
};
