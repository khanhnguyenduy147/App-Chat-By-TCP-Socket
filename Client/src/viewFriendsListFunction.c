#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// include các thư viện cần thiết cho code bên dưới
#define MAX_LINE_LENGTH 256
#define MAX_BUFFER_SIZE 1024

void displayFriendList(const char *friendList)
{
    char *token;
    char *saveptr; // For strtok_r

    // Use strtok_r instead of strtok for thread safety
    token = strtok_r((char *)friendList, "\n", &saveptr);

    while (token != NULL)
    {
        // Display user ID
        printf("User ID: %s\n", token);

        // Move to the next token
        token = strtok_r(NULL, "\n", &saveptr);

        // Check if there is a status token
        if (token != NULL)
        {
            // Display status based on the value
            int status = atoi(token);
            if (status == 1)
            {
                printf("Status: Online\n");
            }
            else if (status == 0)
            {
                printf("Status: Offline\n");
            }
            else
            {
                printf("Invalid status\n");
            }

            // Move to the next token
            token = strtok_r(NULL, "\n", &saveptr);
        }
    }
}

void viewFriendsListFunction(int clientSocket)
{
    /// Gửi thông điệp "get_friend_list\r\n" tới server
    send(clientSocket, "get_friend_list\r\n", strlen("get_friend_list\r\n"), 0);
    printf("Send done!\n");
    // Nhận mã giao thức từ server
    char protocol[MAX_LINE_LENGTH];
    ssize_t bytesRead = recv(clientSocket, protocol, sizeof(protocol), 0);
    if (bytesRead <= 0)
    {
        perror("Error receiving protocol from server");
        return;
    }

    // Nếu mã giao thức là 2012 và danh sách không trống
    if (strcmp(protocol, "2012") == 0)
    {
        send(clientSocket, "OK\r\n", strlen("OK\r\n"), 0);
        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead;

        // Read the size message
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0)
        {
            perror("Error receiving size message");
            return;
        }

        // Null-terminate the received data
        buffer[bytesRead] = '\0';
        // printf("%s", buffer);
        displayFriendList(buffer);

        // Đối với mục đích thực tế, bạn có thể thực hiện xử lý tiếp theo ở đây
    }
    else
    {
        printf("Server response: %s\n", protocol);
    }
}