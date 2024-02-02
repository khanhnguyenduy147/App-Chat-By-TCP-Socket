// Include necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/log.h"
#include "../lib/sessionManager.h"
#include "../lib/limit_local.h"

// Define the maximum length of a line
#define MAX_LINE_LENGTH 256

void getFriendListAndStatus(char *user_id, int socket_fd, const struct Session **sessionList)
{
    // Open the friend file for reading
    FILE *friendFile = fopen("../database/friend.txt", "r");
    if (!friendFile)
    {
        perror("Error opening friend.txt");
        logResponse("4012", socket_fd);
        send(socket_fd, "4012", sizeof("4012"), 0);
        return;
    }
    char *user_id_self = findUserIdBySocket(*sessionList, socket_fd);
    // Kiểm tra đăng nhập hay chưa
    if (user_id_self == NULL)
    {
        // Chưa đăng nhập
        logResponse("4512", socket_fd);
        send(socket_fd, "4512", sizeof("4512"), 0);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char response[MAX_LINE_LENGTH];
    size_t responseSize = 0;
    int totalFriends = 0;

    // Iterate through the friend.txt file
    while (fgets(line, sizeof(line), friendFile) != NULL)
    {
        char friend1[MAX_LINE_LENGTH];
        char friend2[MAX_LINE_LENGTH];
        if (sscanf(line, "%s %s", friend1, friend2) != 2)
        {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        if (strcmp(user_id, friend1) == 0)
        {
            int status = findSocketIdByUserId(*sessionList, friend2) != -1 ? 1 : 0;
            responseSize += snprintf(response + responseSize, MAX_LINE_LENGTH, "\n%s\n%d", friend2, status);
            totalFriends++;
        }
        else if (strcmp(user_id, friend2) == 0)
        {
            int status = findSocketIdByUserId(*sessionList, friend1) != -1 ? 1 : 0;
            responseSize += snprintf(response + responseSize, MAX_LINE_LENGTH, "\n%s\n%d", friend1, status);
            totalFriends++;
        }
    }

    fclose(friendFile);

    // Send the size message back to the client
    if (totalFriends == 0)
    {
        logResponse("2112", socket_fd);
        send(socket_fd, "2112", sizeof("2112"), 0);
    }
    else
    {
        logResponse("2012", socket_fd);
        send(socket_fd, "2012", sizeof("2012"), 0);
        char size_message[MAX_LINE_LENGTH];
        recv(socket_fd, size_message, 5, 0);
        logRequest(size_message, socket_fd);
        //  Send the response back to the client
        logResponse(response, socket_fd);
        send(socket_fd, response, responseSize, 0);
    }
}
