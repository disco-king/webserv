#include "includes.hpp"

int main()
{
	sockaddr_in address;
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(listen < 0){
		perror("listen");
		exit(1);
	}

	int yes = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(0);
	address.sin_port = htons(8000);

	memset(address.sin_zero, '\0', sizeof(address.sin_zero));

	if(bind(listen_fd, (sockaddr *)&address, sizeof(address)) < 0){
		perror("bind");
		exit(1);
	}
	if (listen(listen_fd, 0) < 0){
		perror("listen");
		exit(1);
	}

	std::cout << "accepting...\n";
	int addrlen = sizeof(address);
	int input_fd = accept(listen_fd, (sockaddr *)&address, (socklen_t *)&addrlen);
	if(input_fd < 0){
		perror("accept");
		exit(1);
	}

	std::ofstream file;
	file.open("../moved.mp3", std::ios::out | std::ios::binary);
	if(file.fail()){
		perror("open");
		exit(1);
	}

	char buffer[512];
	int res;
	size_t received = 0;

	while(1){
		res = read(input_fd, buffer, sizeof(buffer));
		if(res < 0){
			perror("read");
			break;
		}
		if(res == 0){
			std::cout << "eof reached\n";
			break;
		}
		received += res;

		file.write(buffer, res);
		if(file.fail()){
			perror("write");
			break;
		}
	}
	file.close();	
	std::cout << "received " << received << " bytes\n";
}
