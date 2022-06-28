// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd;
    fd_set rfds;
    struct timeval tv;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[30000] = {0};
    t_list *lst;
    
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nHello from server!";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    lst = lstnew(&server_fd);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
											(socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}


typedef struct	s_list
{
void			*content;
struct s_list	*next;
}				t_list;

t_list	*lstnew(void *content)
{
	t_list *new;
	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return(NULL);
	new->content = content;
	new->next = NULL;
	return(new);
}

void lstadd(t_list *head, t_list *new)
{
    while(head->next)
        head = head->next;
    head->next = new;
}

void lstremove(t_list *prev)
{
    t_list *buff = prev->next;
    prev->next = prev->next->next;
    free(buff);
}

t_list *findprev(t_list *head, int fd)
{
    while(head->next)
    {
        if(*(int *)(head->next->content) == fd)
            break;
        head = head->next;
    }
    return head->next;
}