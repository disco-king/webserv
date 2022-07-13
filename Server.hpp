#pragma once

#include "Listener.hpp"
#include <sys/select.h>
#include <vector>

class Server
{

private:
	std::map<int, Listener> _listeners;
	std::vector<int> _to_write;
	std::map<int, Listener*> _connections;
	fd_set _fds;
	int _max_fd;

public:
	int init(std::vector<short> const &ports);
	void select();
};