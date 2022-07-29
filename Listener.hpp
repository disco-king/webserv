#pragma once

#include <netinet/in.h>
#include <map>
#include <iostream>
#include "utils.hpp"
#include "./config/Config.hpp"

#define PACK_SIZE 65536

class Listener
{

private:
	int _listen_fd;
	int _addrlen;
	t_listen _listen;
	sockaddr_in _address;
	std::string _response;
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

public:
	Listener(Config& config);
	// ~Listener();
	Listener(Listener const& other);

	int init(short port,
			unsigned int host = INADDR_ANY, 
			int queue = 10);
	int read(int socket);
	int write(int socket);
	void close(int socket);
	int accept();
	int getFD();
};
