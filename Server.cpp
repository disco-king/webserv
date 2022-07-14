#include "Server.hpp"
#include <cstring>

int Server::init(std::vector<short> const &ports)
{
	int fd, res;
	_max_fd = 0;

	std::vector<short>::const_iterator end = ports.end();
	for (std::vector<short>::const_iterator it = ports.begin(); it != end; ++it){
		Listener lst;

		res = lst.init(*it);

		if(res == -1){
			continue;
		}

		fd = lst.getFD();
		_listeners[fd] = lst;
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

		res = ::select(_max_fd + 1, &rfds, &wrfds, NULL, NULL);

		end = _to_write.end();
		for(std::set<int>::const_iterator it = _to_write.begin(); it != end; ++it){
			if(!FD_ISSET(*it, &wrfds))
				continue;
			res = _listeners[*it].write(*it); 
		}

		std::map<int, Listener*>::iterator conn_end = _connections.end();
		for(std::map<int, Listener*>::iterator it = _connections.begin(); it != conn_end; ++it){
			if(!FD_ISSET(it->first, &wrfds))
				continue;
			res = it->second->read(it->first);
			if(res <= 0){
				if(res == 0)
					_to_write.insert(it->first);
				FD_CLR(it->first, &_fds);
				FD_CLR(it->first, &rfds);
				_connections.erase(it->first);
			}
		}

		std::map<int, Listener>::iterator lst_end = _listeners.end();
		for(std::map<int, Listener>::iterator it = _listeners.begin(); it != lst_end; ++it){
			if(!FD_ISSET(it->first, &rfds))
				continue;
			res = it->second.accept();
			if(res != -1)
				_connections[res] = &(it->second);
			res = 0;
		}
	}
}