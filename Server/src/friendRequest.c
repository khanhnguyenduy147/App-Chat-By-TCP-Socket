#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/sessionManager.h"
#define MAX_LINE_LENGTH 256

void friendRequest(char *message, int socket_fd, const struct Session **sessionList)
{
    char senderId[256];
    char receiverId[256];

    if (sscanf(message, "%s\n%s\r\n", senderId, receiverId) == 2)
    {

        int socket_client = findSocketIdByUserId(*sessionList, receiverId);

        FILE *file = fopen("../database/friend.txt", "r");
        if (file == NULL)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        char line[256];
        char friendId1[MAX_LINE_LENGTH];
        char friendId2[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (sscanf(line, "%s %s", friendId1, friendId2) != 2)
            {
                fprintf(stderr, "Error parsing line: %s\n", line);
                continue;
            }

            if ((strcmp(friendId1, senderId) == 0 && strcmp(friendId2, receiverId) == 0) || (strcmp(friendId1, receiverId) == 0 && strcmp(friendId2, senderId) == 0))
            {
                send(socket_fd, "4108", sizeof("4108"), 0);
            }
            else
            {
                char send_message[512];

                sprintf(send_message, "2008/n%s", senderId);

                send(socket_client, send_message, sizeof(send_message), 0);
                send(socket_fd, "2008", sizeof("2008"), 0);
            }
        }
    }
}
