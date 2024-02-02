#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 2048
#define MAX_LINE_LENGTH 256

void getContentGroup(char *idgroup, int clientSocket)
{
    // Gửi yêu cầu lấy nội dung tin nhắn của nhóm đến Server
    char request[MAX_LINE_LENGTH];
    snprintf(request, sizeof(request), "get_content_message_of_group\n%s\r\n", idgroup);
    send(clientSocket, request, strlen(request), 0);

    // Nhận mã giao thức từ Server
    char protocolCode[MAX_LINE_LENGTH];
    recv(clientSocket, protocolCode, sizeof(protocolCode), 0);
    protocolCode[4] = '\0';

    if (strcmp(protocolCode, "2016") == 0)
    {
        // Lấy thành công, gửi "OK" lên Server
        send(clientSocket, "OK\r\n", sizeof("OK\r\n"), 0);

        // Nhận và hiển thị dữ liệu tin nhắn
        char messageData[MAX_MESSAGE_LENGTH];
        recv(clientSocket, messageData, sizeof(messageData), 0);
        printf("%s\n", messageData);
    }
    else if (strcmp(protocolCode, "4016") == 0)
    {
        // Lấy thất bại
        printf("Failed to retrieve messages for group %s\n", idgroup);
    }
    else
    {
        // Xử lý các mã giao thức khác nếu cần
        printf("Unexpected protocol code: %s\n", protocolCode);
    }
}