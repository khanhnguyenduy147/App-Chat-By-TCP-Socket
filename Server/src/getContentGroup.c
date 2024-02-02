#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/log.h"

#define MAX_LINE_LENGTH 256
#define MAX_MESSAGE_LENGTH 1024

void getContentGroup(char *user_id, int socket_fd, char *message)
{
    // Nhận groupid từ client
    char groupid[MAX_LINE_LENGTH];
    sscanf(message, "%s", groupid);

    // Tạo tên file từ groupid
    char filename[MAX_LINE_LENGTH];
    snprintf(filename, sizeof(filename), "../database/message_group/%s.txt", groupid);

    // Mở file để kiểm tra sự tồn tại của nó
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        // Gửi mã giao thức 4016 nếu file không tồn tại
        logResponse("4016", socket_fd);
        send(socket_fd, "4016", sizeof("4016"), 0);
        return;
    }
    // Gửi mã giao thức 2016 nếu file tồn tại
    logResponse("2016", socket_fd);
    send(socket_fd, "2016", sizeof("2016"), 0);
    // Nhận "OK" từ client
    char okMessage[MAX_LINE_LENGTH];
    recv(socket_fd, okMessage, sizeof(okMessage), 0);
    logResponse(okMessage, socket_fd);
    okMessage[3] = '\0';
    if (strcmp(okMessage, "OK\r") != 0)
    {
        // Đảm bảo client gửi "OK"
        printf("Unexpected response from client: %s", okMessage);
        fclose(file);
        return;
    }

    // Gửi nội dung của file groupid.txt cho client
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Đọc toàn bộ nội dung file vào một buffer
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer)
    {
        perror("Error allocating memory");
        fclose(file);
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0'; // Đảm bảo buffer kết thúc bằng null terminator

    // Gửi toàn bộ buffer qua socket
    logResponse(buffer, socket_fd);
    send(socket_fd, buffer, file_size, 0);

    // Giải phóng bộ nhớ đã cấp phát cho buffer
    free(buffer);

    // Đóng file sau khi đã đọc xong
    fclose(file);
}
