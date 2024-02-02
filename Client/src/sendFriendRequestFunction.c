#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#define BUFFER_SIZE 256

int sendFriendRequestFunction(int clientSocket)
{
    char user_id_received[256];

    printf("Nhap id nguoi muon ket ban ");
    scanf("%s", user_id_received);

    // Tạo thông điệp gửi lời mời kết bạn
    char message[512];
    sprintf(message, "add_friend\n%s\r\n", user_id_received);

    send(clientSocket, message, strlen(message), 0);

    char response[BUFFER_SIZE];
    if (recv(clientSocket, response, BUFFER_SIZE, 0) == -1)
    {
        perror("Error receiving response from server");
        return 0; // Trả về 0 để báo lỗi
    }

    // Xử lý phản hồi từ serve
    if (strcmp(response, "2008") == 0)
    {
        printf("Gui loi moi thanh cong\n");
        // Thực hiện các công việc cần thiết sau khi gửi lời mời thành công
    }
    else if (strcmp(response, "4008") == 0)
    {
        printf("Khong tim thay ID hoac nguoi dung khong online\n");
        // Thực hiện các công việc cần thiết khi không tìm thấy ID hoặc người dùng không online
    }
    else if (strcmp(response, "4108") == 0)
    {
        printf("Da co ket ban tu truoc\n");
        // Thực hiện các công việc cần thiết khi đã có kết bạn từ trước
    }
}