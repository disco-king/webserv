#pragma once

#include "Interface.hpp"
#include "../config/Config.hpp"
#include <sys/select.h>
#include <set>
#include <vector>

class Server
{

private:
	std::map<int, Interface> _inters;
	std::set<int> _to_write;
	std::map<int, Interface*> _connections;
	fd_set _fds;
	int _max_fd;
	Config & _config;

	void _clearConnections();
	void _readLoop(fd_set &rfds);
	void _writeLoop(fd_set &rfds, fd_set &wrfds);
	void _acceptLoop(fd_set &rfds);

	Server();
	Server(Server const&);

public:
	Server(Config &config);
	int init();
	void run();
};