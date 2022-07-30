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

	std::vector<t_listen> listeners = _config.getListeners();
	std::vector<t_listen>::const_iterator end = listeners.end();
	for (std::vector<t_listen>::const_iterator it = listeners.begin(); it != end; ++it){
		Listener listener(_config);

		res = listener.init(it->port);

		if(res == -1){
			continue;
		}

		fd = listener.getFD();
		std::cout << "on port " << it->port
		<< " got fd " << fd << '\n';
		_listeners.insert(std::make_pair(fd, listener));
		FD_SET(fd, &_fds);
		_max_fd = std::max(_max_fd, fd);
	}

	if(_max_fd == 0){
		std::cerr << "error: server init\n";
		return -1;
	}
	return 0;
}

void Server::select()
{
	int res;
	fd_set rfds;
	fd_set wrfds;

	while(1){
		FD_ZERO(&rfds);
		FD_ZERO(&wrfds);

		memcpy(&rfds, &_fds, sizeof(fd_set));
		std::set<int>::const_iterator end = _to_write.end();
		for(std::set<int>::const_iterator it = _to_write.begin(); it != end; ++it)
			FD_SET(*it, &wrfds);

		std::cout << "\nwaiting for connections\n\n";
		res = ::select(_max_fd + 1, &rfds, &wrfds, NULL, NULL);

		if(res < 0){
			std::cerr << "error: select\n";
			for(std::map<int, Listener*>::iterator it = _connections.begin();
							it != _connections.end(); ++it)
				it->second->close(it->first);
			FD_ZERO(&_fds);
			FD_ZERO(&rfds);
			_connections.clear();
			_to_write.clear();
			for(std::map<int, Listener>::iterator it = _listeners.begin();
							it != _listeners.end(); ++it)
				FD_SET(it->first, &_fds);
			continue;
		}

		end = _to_write.end();
		for(std::set<int>::const_iterator it = _to_write.begin(); it != end; ++it){
			if(!FD_ISSET(*it, &wrfds))
				continue;
			std::cout << "write: socket " << *it << '\n';
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

		std::map<int, Listener*>::iterator conn_end = _connections.end();
		for(std::map<int, Listener*>::iterator it = _connections.begin(); it != conn_end; ++it){
			if(!FD_ISSET(it->first, &rfds))
				continue;
			std::cout << "read: socket " << it->first << '\n';
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

		std::map<int, Listener>::iterator lst_end = _listeners.end();
		for(std::map<int, Listener>::iterator it = _listeners.begin(); it != lst_end; ++it){
			if(!FD_ISSET(it->first, &rfds))
				continue;
			std::cout << "accept: socket " << it->first << '\n';
			res = it->second.accept();
			if(res != -1){
				FD_SET(res, &_fds);
				_max_fd = std::max(_max_fd, res);
				_connections[res] = &(it->second);
			}
			break;
		}
	}
}