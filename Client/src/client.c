#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../lib/function_chat.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 2048

void displayMenu01()
{
    printf("\n----- Menu -----\n");
    printf("1. Đăng Ký\n");
    printf("2. Đăng nhập\n");
    printf("0. Thoát\n");
    printf("Nhập lựa chọn của bạn: ");
}

void displayMenu02()
{
    printf("\n----- Menu -----\n");
    printf("1. Gửi lời mời kết bạn\n");
    printf("2. Thông báo lời mời kết bạn\n");
    printf("3. Xem danh sách bạn bè\n");
    printf("4. Chat giữa hai người\n");
    printf("5. Tạo nhóm chat\n");
    printf("6. Thêm người dùng khác vào nhóm chat\n");
    printf("7. Xóa người dùng khỏi nhóm chat\n");
    printf("8. Rời nhóm chat\n");
    printf("9. Chat trong nhóm\n");
    printf("10. Ngắt kết nối\n");
    printf("11. Xem danh sách nhóm của bản thân\n");
    printf("0. Thoát\n");
    printf("Nhập lựa chọn của bạn: ");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP_Addr> <Port_Number>\n", argv[0]);
        return 1;
    }

    int clientSocket;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Initialize server address structure
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]); // IP address of the server

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("connect");
        exit(1);
    }

    // Receive initial server response
    bytes_received = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (strstr(buffer, "2000") == buffer)
    {
        printf("Connect server!\n");
    }
    int login = 0;
    int choice;
    do
    {
        displayMenu01();
        scanf("%d", &choice);
        // Đọc và loại bỏ ký tự mới dòng từ bộ đệm
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        // Khi người dùng chưa đăng nhập
        switch (choice)
        {
        case 1:
        {
            // 1. Gửi lời mời kết bạn
            registrationFunction(clientSocket);
            break;
            // Dăng ký xong vẫn phải yêu cầu đăng nhập hoặc đăng ký
        }
        case 2:
        {
            login = loginFunction(clientSocket);
            // ...
            break;
        }
        // Thêm các trường hợp xử lý cho các chức năng khác tại đây
        case 0:
        {
            printf("Thoát khỏi ứng dụng.\n");
            break;
        }
        default:
        {
            printf("Lựa chọn không hợp lệ.\n");
        }
        }

    } while ((choice != 0) && (login != 1));

    // Khi người dùng đã đăng nhập
    while (login == 1)
    {
        /* code */
        displayMenu02();
        scanf("%d", &choice);
        // Đọc và loại bỏ ký tự mới dòng từ bộ đệm
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        switch (choice)
        {
        case 1:
        {
            // Gửi lời mời kết bạn
            sendFriendRequestFunction(clientSocket);
            break;
        }
        case 2:
        {
            // Thông báo lời mời kết bạn
            // ...
            break;
        }
        case 3:
        {
            viewFriendsListFunction(clientSocket);
            // ...
            break;
        }
        case 4:
        {
            // Chat giữa hai người
            // ...
            break;
        }
        case 5:
        {
            // Tạo nhóm chat
            // ...
            break;
        }
        case 6:
        {
            // Thêm người dùng khác vào nhóm chat
            addMembersToGroupFunction(clientSocket);
            break;
        }
        case 7:
        {
            // Xóa người dùng khỏi nhóm chat
            deleteMemberOfGroup(clientSocket);
            break;
        }
        case 8:
        {
            // Rời nhóm chat
            // ...
            break;
        }
        case 9:
        {
            // Chat trong nhóm
            chatGroup(clientSocket);
            break;
        }
        case 10:
        {
            // Ngắt kết nối
            disconnectFunction(clientSocket);
        }
        case 11:
        {
            // Xem danh sách nhóm
            // getListGroup(clientSocket);
        }
        // Thêm các trường hợp xử lý cho các chức năng khác tại đây
        case 0:
        {
            printf("Thoát khỏi ứng dụng.\n");
            break;
        }
        default:
        {
            printf("Lựa chọn không hợp lệ.\n");
        }
        }
    }

    // Đóng kết nối
    close(clientSocket);

    return 0;
}
