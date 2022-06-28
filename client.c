// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

#ifndef FOREIGN

#define FOREIGN 1

#endif

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    char *hello;

	if(FOREIGN)
		hello = "Hello from foreign client";
	else
		hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
	else if(FOREIGN)
        printf("\n Socket created\n");
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(!FOREIGN && inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0 //local connections
    || FOREIGN && inet_pton(AF_INET, "192.168.43.197", &serv_addr.sin_addr)<=0)//foreign connections
    {
        printf("\n Invalid address/ Address not supported \n");
        return -1;
    }
	else if(FOREIGN)
        printf("\n Convertion done, connecting\n");
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return -1;
    }
	else if(FOREIGN)
        printf("\n Connected \n");

    for (int i = 0; i < 5; i++)
    {
        send(sock, hello, strlen(hello), 0 );
        printf("Hello message sent\n");
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
        // sleep(1);
    }
	return 0;
}
