#pragma once

#include "Interface.hpp"
#include "./config/Config.hpp"
#include <sys/select.h>
#include <set>
#include <vector>

class Server
{

private:
	std::map<int, Interface> _listeners;
	std::set<int> _to_write;
	std::map<int, Interface*> _connections;
	fd_set _fds;
	int _max_fd;
	Config & _config;

	Server();
	Server(Server const&);

public:
	Server(Config &config);
	int init();
	void select();
};