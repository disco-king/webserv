#include "Server.hpp"
#include <cstring>

Server::Server(Config &config) : _config(config)
{
	init();
}


int Server::init()
{
	int fd, res;
	_max_fd = 0;
	FD_ZERO(&_fds);

	std::vector<t_listen> listeners = _config.getListeners();
	std::vector<t_listen>::const_iterator end = listeners.end();
	for (std::vector<t_listen>::const_iterator it = listeners.begin(); it != end; ++it){
		Interface listener(_config);

		res = listener.init(*it);

		if(res == -1){
			continue;
		}

		fd = listener.getFD();
		_inters.insert(std::make_pair(fd, listener));
		FD_SET(fd, &_fds);
		_max_fd = std::max(_max_fd, fd);
	}

	if(_max_fd == 0){
		std::cerr << "Error: cound't initialize any servers\n";
		return -1;
	}
	return 0;
}

void Server::_clearConnections()
{
	std::cerr << "Error: select\n";
	std::map<int, Interface*>::iterator cons_end = _connections.end();
	for(std::map<int, Interface*>::iterator it = _connections.begin();
					it != cons_end; ++it)
		it->second->close(it->first);
	FD_ZERO(&_fds);
	_connections.clear();
	_to_write.clear();
	_max_fd = 0;
	std::map<int, Interface>::iterator ints_end = _inters.end();
	for(std::map<int, Interface>::iterator it = _inters.begin();
					it != ints_end; ++it){
		FD_SET(it->first, &_fds);
		_max_fd = std::max(it->first, _max_fd);
	}
}

void Server::_readLoop(fd_set &rfds)
{
	int res;
	std::map<int, Interface*>::iterator conn_end = _connections.end();
	for(std::map<int, Interface*>::iterator it = _connections.begin(); it != conn_end; ++it){
		if(!FD_ISSET(it->first, &rfds))
			continue;
		res = it->second->read(it->first);
		if(res < 0){
			FD_CLR(it->first, &_fds);
			FD_CLR(it->first, &rfds);
			_connections.erase(it->first);
		}
		if(res == 0)
			_to_write.insert(it->first);
		break;
	}
}

void Server::_writeLoop(fd_set &rfds, fd_set &wrfds)
{
	int res;
	std::set<int>::const_iterator end = _to_write.end();
	for(std::set<int>::const_iterator it = _to_write.begin(); it != end; ++it){
		if(!FD_ISSET(*it, &wrfds))
			continue;
		res = _connections[*it]->write(*it);
		if(res == 0){
			_to_write.erase(*it);
		}
		if(res < 0){
			FD_CLR(*it, &_fds);
			FD_CLR(*it, &rfds);
			_connections.erase(*it);
		}
		break;
	}
}

void Server::_acceptLoop(fd_set &rfds)
{
	int res;
	std::map<int, Interface>::iterator lst_end = _inters.end();
	for(std::map<int, Interface>::iterator it = _inters.begin(); it != lst_end; ++it){
		if(!FD_ISSET(it->first, &rfds))
			continue;
		res = it->second.accept();
		if(res != -1){
			FD_SET(res, &_fds);
			_max_fd = std::max(_max_fd, res);
			_connections[res] = &(it->second);
		}
	}
}

void Server::run()
{
	int res;
	fd_set rfds;
	fd_set wrfds;

	std::cout << GRAY << "<<< Running >>> " << RESET << '\n';
	while(1){
		FD_ZERO(&rfds);
		FD_ZERO(&wrfds);

		memmove(&rfds, &_fds, sizeof(fd_set));
		std::set<int>::const_iterator end = _to_write.end();
		for(std::set<int>::const_iterator it = _to_write.begin(); it != end; it++)
			FD_SET(*it, &wrfds);

		res = ::select(_max_fd + 1, &rfds, &wrfds, NULL, NULL);
		if(res < 0){
			_clearConnections();
			continue;
		}
		_writeLoop(rfds, wrfds);
		_readLoop(rfds);
		_acceptLoop(rfds);
	}
}