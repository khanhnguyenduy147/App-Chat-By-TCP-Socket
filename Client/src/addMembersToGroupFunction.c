// include các thư viện cần thiết cho code bên dưới
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum length of a line
#define MAX_LINE_LENGTH 256
void getListGroup(int clientSocket);
void viewFriendsListFunction(int clientSocket);
void addMembersToGroupFunction(int clientSocket)
{
    // viewFriendsListFunction(clientSocket);
    //  getListGroup(clientSocket);
    //  View friends list

    // Input user ID and group ID
    char userId[MAX_LINE_LENGTH];
    char groupId[MAX_LINE_LENGTH];

    printf("Enter the user ID: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0'; // Remove the newline character

    printf("Enter the group ID: ");
    fgets(groupId, sizeof(groupId), stdin);
    groupId[strcspn(groupId, "\n")] = '\0'; // Remove the newline character

    // Create the message
    char message[MAX_LINE_LENGTH];
    snprintf(message, sizeof(message), "add_user_to_group\n%s\n%s\r\n", groupId, userId);

    // Send the message to the server
    send(clientSocket, message, strlen(message), 0);

    // Receive the protocol code from the server
    char protocolCode[5]; // Assuming a 4-digit protocol code
    recv(clientSocket, protocolCode, 10, 0);
    protocolCode[4] = '\0';
    printf("Code:%s\n", protocolCode);
    // Process the protocol code
    if (strcmp(protocolCode, "2005") == 0)
    {
        printf("Add member to group success\n");
    }
    else if (strcmp(protocolCode, "4105") == 0)
    {
        printf("Add member to group not success: No user ID in friend list\n");
    }
    else if (strcmp(protocolCode, "4205") == 0)
    {
        printf("Add member to group not success: No group ID\n");
    }
    else if (strcmp(protocolCode, "4305") == 0)
    {
        printf("Add member to group not success: Already in the group\n");
    }
    else if (strcmp(protocolCode, "4405") == 0)
    {
        printf("Add member to group not success: Invalid user ID\n");
    }
    else
    {
        printf("Invalid protocol code\n");
    }
}