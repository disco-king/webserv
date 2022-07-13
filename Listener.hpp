#pragma once

#include <netinet/in.h>
#include <map>
#include <iostream>
#include "utils.hpp"

#define PACK_SIZE 65536

class Listener
{

private:
	int _listen_fd;
	int _addrlen;
	listen_socket _listen;
	sockaddr_in _address;
	std::string _response;
	std::map<int, std::string> _sockets;
	std::map<int, size_t> _written;

public:
	Listener();
	~Listener();
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