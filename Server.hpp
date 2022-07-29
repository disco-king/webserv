#pragma once

#include "Listener.hpp"
#include "./config/Config.hpp"
#include <sys/select.h>
#include <set>
#include <vector>

class Server
{

private:
	std::map<int, Listener> _listeners;
	std::set<int> _to_write;
	std::map<int, Listener*> _connections;
	fd_set _fds;
	int _max_fd;
	Config & _config;

public:
	Server(Config &config);
	int init();
	void select();
};