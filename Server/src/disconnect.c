#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

void disconnect(char *message, int socket_fd)
{
    char userId1[256];
    char userId2[256];

    if (sscanf(message, "\n%s\n%s\n\r", userId1, userId2) == 2)
    {

        char filePath1[256];
        char filePath2[256];
        sprintf(filePath1, "../database/message_private/%s_%s.txt", userId1, userId2);
        sprintf(filePath2, "../database/message_private/%s_%s.txt", userId2, userId1);

        if (remove(filePath1) == 0 || remove(filePath2) == 0)
        {

            send(socket_fd, "2015", sizeof("2015"), 0);
        }
        else
        {

            send(socket_fd, "4015", sizeof("4015"), 0);
        }
    }
}
