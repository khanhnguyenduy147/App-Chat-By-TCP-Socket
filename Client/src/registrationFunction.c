// include các thư viện cần thiết cho code bên dưới
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256
#define MAX_LENGTH 60
// dang ky
int registrationFunction(int clientSocket)
{
    char fullname[MAX_LENGTH];
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("Nhap ho ten: ");
    fgets(fullname, sizeof(fullname), stdin);
    fullname[strcspn(fullname, "\n")] = '\0'; // Loại bỏ ký tự '\n' thừa

    // Kiểm tra chiều dài của tên người dùng
    if (strlen(fullname) >= MAX_LENGTH)
    {
        printf("Ten qua dai. Vui long nhap lai.\n");
        return 0; // Trả về 0 để báo lỗi
    }

    printf("Nhap tai khoan: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Loại bỏ ký tự '\n' thừa

    // Kiểm tra chiều dài của tên người dùng
    if (strlen(username) >= MAX_LENGTH)
    {
        printf("Tai khoan qua dai. Vui long nhap lai.\n");
        return 0; // Trả về 0 để báo lỗi
    }

    printf("Nhập mật khẩu: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Loại bỏ ký tự '\n' thừa

    // Kiểm tra chiều dài của mật khẩu
    if (strlen(password) >= MAX_LENGTH)
    {
        printf("Mật khẩu quá dài. Vui lòng nhập lại.\n");
        return 0; // Trả về 0 để báo lỗi
    }

    // Gửi thông điệp đăng nhập lên server
    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "register\n%s\n%s\n%s\r\n", fullname, username, password);
    if (send(clientSocket, message, strlen(message), 0) == -1)
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
    if (strcmp(response, "2001") == 0)
    {
        printf("Dang ky thanh cong");
        return 1; // Trả về 1 nếu đăng nhập thành công
    }
    else
    {
        printf("Dang ky that bai");
        return 0; // Trả về 0 nếu đăng nhập thất bại
    }
}