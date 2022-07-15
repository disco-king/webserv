#include "Listener.hpp"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>

Listener::Listener()
{
	_response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nHello from server!";
}

int Listener::init(short port, unsigned int host, int queue)
{
	int res;

	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_listen_fd == 0){
		std::cerr << "error: socket creation\n";
		return -1;
	}

	int yes = 1;
	setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(host);
	_address.sin_port = htons(port);

	memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));

	if(bind(_listen_fd, (sockaddr *)&_address, sizeof(_address)) < 0){
		std::cerr << "error: bind\n";
		return -1;
	}
	if (listen(_listen_fd, queue) < 0){
		std::cerr << "error: listen\n";
		return -1;
	}

	_addrlen = sizeof(_address);
	return 0;
}

int Listener::accept()
{
	int new_socket = ::accept(_listen_fd, (sockaddr*)&_address,
									(socklen_t*)&_addrlen);
	if(new_socket <= 0)
		std::cerr << "error: accept\n";
	else{
		std::cout << "new connection on socket " << new_socket << '\n';
		_sockets[new_socket] = "";
	}
	return new_socket;
}

int Listener::read(int socket)
{
	char buff[PACK_SIZE] = {0};

	int ret = ::read(socket, buff, PACK_SIZE);
	if(ret <= 0){
		close(socket);
		if(ret < 0){
			std::cerr << "error: socket " << socket << '\n';
		}
		std::cout << "client on socket " << socket << " closed connection\n";
		return -1;
	}
	_sockets[socket] += buff;
	// return 1; //for proper message length handling
	return 0;
}

int Listener::write(int socket)
{
	if(_written.count(socket) == 0)
		_written[socket] = 0;

	std::string to_send = _response.substr(_written[socket],
											PACK_SIZE);
	int ret = ::write(socket, to_send.c_str(), to_send.length());

	if(ret == -1){
		std::cerr << "error: write\n";
		close(socket);
		return -1;
	}

	// _written[socket] += ret;//we'll need that when responses get big
	// if(_written[socket] >= _response.size()){
	// 	// close(socket);			//proper write size handling
	// 	// _written.erase(socket);	//coming soon
	// 	return 0;
	// }
	// return 1;
	return 0;
}

void Listener::close(int socket)
{
	::close(socket);
	this->_sockets.erase(socket);
}

int Listener::getFD()
{
	return _listen_fd;
}
