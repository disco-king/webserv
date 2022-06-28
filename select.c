#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
    fd_set rfds;
    struct timeval tv;
    int ret;
    char buff[1024];

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    tv.tv_sec = 5;
    tv.tv_usec = 0;


    ret = select(1, &rfds, NULL, NULL, &tv);

    if(ret == -1)
        perror("select");
    else if (ret)
    {
        printf("Data to be read in %d fd(s)\n", ret);
        ret = read(0, buff, sizeof(buff));
        buff[ret] = 0;
        printf("Data read: %s\n", buff);
    }
    else
        printf("No data received\n");

    exit(EXIT_FAILURE);
}
