#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include<unistd.h>
#define BUFFER_SIZE 256

void disconnectFunction(int clientSocket){
char user_id_received[256];
    printf("Nhap id nguoi muon ngat ket noi ");
    scanf("%s", user_id_received);

    // Tạo thông điệp gửi lời mời kết bạn
    char message[512];
    sprintf(message, "disconnect\n%s\r\n", user_id_received);

    send(clientSocket, message, strlen(message), 0);

     char response[BUFFER_SIZE];
    if (recv(clientSocket, response, BUFFER_SIZE, 0) == -1)
    {
        perror("Error receiving response from server");
        return; // Trả về 0 để báo lỗi
    }
    // Xử lý phản hồi từ serve
        if (strcmp(response, "2015") == 0) {
            printf("Ngat ket noi thanh cong\n");
            // Thực hiện các công việc cần thiết sau khi gửi lời mời thành công
        } else if (strcmp(response, "4015") == 0) {
            printf("Khong tim thay ID hoac nguoi dung chua ket ban\n");
            // Thực hiện các công việc cần thiết khi không tìm thấy ID 
        }
}