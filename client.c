// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8000

#ifndef FOREIGN

#define FOREIGN 0

#endif

int sock; 

void handler(int num)
{
	write(1, " sigint received, terminating\n", 30);
	close(sock);
	exit(0);
}

int main(int argc, char const *argv[])
{
	long valread;
	struct sockaddr_in serv_addr;
	char *hello;
	int repeats, sleep_time;

	if (argc == 1)
	{
		printf("need a connection type (1, 2 or 3) as arg\n");
		return(1);
	}

	switch (argv[1][0])
	{
	case '1':
		repeats = 3;
		sleep_time = 1;
		break;

	case '2':
		repeats = 1;
		sleep_time = 3;
		break;

	case '3':
		repeats = 1;
		sleep_time = 1000;
		break;

	default:
		break;
	}

	//hello = "GET / HTTP/1.1\r\nHost: localhost:8000\r\n\r\n";
	//hello = "POST /dir/test.txt HTTP/1.1\r\nHost: any\r\nContent-Length: 5\r\n\r\nLorem";
	//hello = "POST /long_post.txt HTTP/1.1\r\nHost: localhost:8000\r\nConnection: keep-alive\r\nTransfer-Encoding: Chunked\r\n\r\n9\r\nChunk one\r\n9\r\nChunk two\r\nB\r\nChunk three\r\n0\r\n\r\n";
	//hello = "DELETE /test.txt HTTP/1.1\r\nHost: localhost:8000\r\nConnection: keep-alive\r\n\r\n";
	char buffer[1024];
	memset(buffer, 0, 1024);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(!FOREIGN && inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0 //local connections
	|| FOREIGN && inet_pton(AF_INET, "192.168.43.39", &serv_addr.sin_addr)<=0)//foreign connections
	{
		printf("\n Invalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Connection failed");
		return -1;
	}

	signal(SIGINT, handler);

	for (int i = 0; i < repeats; i++)
	{
		send(sock, hello, strlen(hello), 0 );
		printf("Hello message sent\n");
		valread = read(sock , buffer, 1024);
		printf("%s\n",buffer );
		sleep(sleep_time);
	}
	close(sock);
	return 0;
}
