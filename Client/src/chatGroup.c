#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGE_LENGTH 2048
#define MAX_LINE_LENGTH 256

void getContentGroup(char *idgroup, int clientSocket);

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void chatGroup(int clientSocket)
{
    // Lấy groupid từ người dùng
    char groupid[MAX_LINE_LENGTH];
    printf("Enter groupid: ");
    fgets(groupid, sizeof(groupid), stdin);
    groupid[strcspn(groupid, "\n")] = '\0'; // Loại bỏ ký tự newline từ đầu vào

    // Gọi hàm getContentGroup để nhận nội dung tin nhắn từ Server
    getContentGroup(groupid, clientSocket);

    // Nhập và gửi tin nhắn cho Server
    char message[MAX_MESSAGE_LENGTH];
    while (1)
    {
        printf("Enter your message ('!q' to exit): ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0'; // Loại bỏ ký tự newline từ đầu vào

        // Kiểm tra điều kiện thoát
        if (strcmp(message, "!q") == 0)
            break;

        // Gửi tin nhắn lên Server
        char sendMessage[MAX_MESSAGE_LENGTH];
        snprintf(sendMessage, sizeof(sendMessage), "send_message_group\n%s\n%s\r\n", groupid, message);
        send(clientSocket, sendMessage, strlen(sendMessage), 0);
        // Nhận và hiển thị mã giao thức từ Server
        char responseCode[MAX_LINE_LENGTH];
        recv(clientSocket, responseCode, sizeof(responseCode), 0);
        responseCode[4] = '\0'; // Đảm bảo null-terminated
        printf("Protocol code: %s\n");
        if (strcmp(responseCode, "2007") == 0)
        {
            printf("Message sent successfully.\n");
        }
        else
        {
            printf("Failed to send message. Server response: %s\n", responseCode);
            break;
        }

        // Xóa bộ đệm đầu vào
        clearInputBuffer();
    }
}
