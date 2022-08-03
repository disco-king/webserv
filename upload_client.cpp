#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <cstring> 
#include <unistd.h>
#include <string>
#include <sys/stat.h>


size_t get_fsize(char *fname)
{
	struct stat data;
	if(stat(fname, &data) < 0)
		return 0;
	return data.st_size;
}

int main(int argc, char **argv)
{
	if(argc < 2){
		std::cout << "provide filename\n";
		exit(1);
	}
	char *fpath = argv[1];

	sockaddr_in serv_address;
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_address, 0, sizeof(sockaddr_in));
	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(8000);
	serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(fd, (sockaddr *)&serv_address, sizeof(sockaddr)) < 0){
		perror("connect");
		exit(1);
	}

	std::ifstream file;
	file.open(fpath, std::ios::binary);
	if(file.fail()){
		perror("open");
		exit(1);
	}

	char buffer[512];
	int res;
	size_t fsize = get_fsize(fpath);
	size_t sent = 0;
	size_t runs = 0;

	std::string head = "POST /test.png HTTP/1.1\r\nHost: localhost:8000\r\nContent-Length: ";
	head += std::to_string(fsize) + "\r\n\r\n";
	res = write(fd, head.c_str(), head.size());
	if(res < 0){
		perror("write");
		file.close();
		exit(1);
	}
	memset(buffer, 0, sizeof(buffer));
	
	while(1){
		file.read(buffer, sizeof(buffer));
		if(file.fail() && !file.eof()){
			perror("file.read()");
			break;
		}

		res = write(fd, buffer, file.gcount());
		if(res < 0){
			perror("write");
			break;
		}
		sent += res;
		++runs;
		
		if(file.eof() || sent >= fsize){
			std::cout << "eof reached\n";
			break;
		}
	}
	file.close();
	std::cout << "sent " << sent << " bytes in "
		<< runs << " runs\n"; 
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, 512);
	std::cout << "GOT RESPONSE:\n\n" << buffer;
	close(fd);
	return 0;	
}
