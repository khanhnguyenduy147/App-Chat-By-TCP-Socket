#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_USERNAME_LENGTH 60
#define MAX_PASSWORD_LENGTH 60
#define BUFFER_SIZE 256
int loginFunction(int clientSocket)
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    // Người dùng nhập tên người dùng
    printf("Nhập tên người dùng: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Loại bỏ ký tự '\n' thừa

    // Kiểm tra chiều dài của tên người dùng
    if (strlen(username) >= MAX_USERNAME_LENGTH)
    {
        printf("Tên người dùng quá dài. Vui lòng nhập lại.\n");
        return 0; // Trả về 0 để báo lỗi
    }

    // Người dùng nhập mật khẩu
    printf("Nhập mật khẩu: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Loại bỏ ký tự '\n' thừa

    // Kiểm tra chiều dài của mật khẩu
    if (strlen(password) >= MAX_PASSWORD_LENGTH)
    {
        printf("Mật khẩu quá dài. Vui lòng nhập lại.\n");
        return 0; // Trả về 0 để báo lỗi
    }

    // Gửi thông điệp đăng nhập lên server
    char loginMessage[BUFFER_SIZE];
    snprintf(loginMessage, sizeof(loginMessage), "login\n%s\n%s\r\n", username, password);

    if (send(clientSocket, loginMessage, strlen(loginMessage), 0) == -1)
    {
        perror("Error sending login message to server");
        return 0; // Trả về 0 để báo lỗi
    }

    // Nhận phản hồi từ server
    char response[BUFFER_SIZE];
    if (recv(clientSocket, response, BUFFER_SIZE, 0) == -1)
    {
        perror("Error receiving response from server");
        return 0; // Trả về 0 để báo lỗi
    }

    // In thông báo từ server
    printf("Server response: %s\n", response);

    // Kiểm tra phản hồi từ server và trả về kết quả
    if (strcmp(response, "2002") == 0)
    {
        return 1; // Trả về 1 nếu đăng nhập thành công
    }
    else
    {
        return 0; // Trả về 0 nếu đăng nhập thất bại
    }
}