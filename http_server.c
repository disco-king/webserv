#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#define PORT 8080

int server_fd;

void handler(int num)
{
	write(1, "sigterm received, terminating\n", 30);
	close(server_fd);
	exit(0);
}

int main(int argc, char const *argv[])
{
    int new_socket, ret;
    int max_fd;
    fd_set main, rfds;
    struct timeval tv;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[30000] = {0};
    char remoteIP[INET_ADDRSTRLEN] = {0};
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nHello from server!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int yes = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FD_SET(server_fd, &main);
    max_fd = server_fd;

    signal(SIGTERM, handler);

    while(1)
    {
        printf("\nselecting\n");
        rfds = main;
        if(select(max_fd + 1, &rfds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(1);
        }

        for(int i = 0; i <= max_fd; i++)
        {
            if(FD_ISSET(i, &rfds))
            {
                if(i == server_fd)
                {
                    printf("going to accept fd %d\n", i);
                    new_socket = accept(server_fd, (struct sockaddr *)&address,
                                    (socklen_t*)&addrlen);
                    if(new_socket < 0)
                        perror("accept");
                    else
                    {
                        FD_SET(new_socket, &main);
                        max_fd = new_socket > max_fd ? new_socket : max_fd;
                        printf("selectserver: new connection from %s on socket %d\n",
                                inet_ntop(address.sin_family, &address.sin_addr,
                                            remoteIP, INET_ADDRSTRLEN),
                                new_socket);
                    }
                }
                else
                {
                    printf("reading from fd %d\n", i); 
                    ret = read(i, buffer, sizeof(buffer));
                    if(ret <= 0)
                    {
                        if(ret < 0)
                            perror("read");
                        printf("closing fd %d\n", i); 
                        close(i);
                        FD_CLR(i, &main);
                    }
                    else
                    {
                        printf("%s\n",buffer );
                        write(i, hello, strlen(hello));
                    }
                }
            }
        }
    }
    return 0;
}
