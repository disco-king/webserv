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
	fd_set to_read;
	fd_set to_write;

	while(1){
		FD_ZERO(&to_read);
		FD_ZERO(&to_write);

		std::vector<int>::const_iterator end = _to_write.end();
		memcpy(&to_read, &_fds, sizeof(fd_set));
		for(std::vector<int>::const_iterator it = _to_write.begin(); it != end; ++it)
			FD_SET(*it, &to_write);

		res = ::select(_max_fd + 1, &to_read, &to_write, NULL, NULL);
		
	}
}