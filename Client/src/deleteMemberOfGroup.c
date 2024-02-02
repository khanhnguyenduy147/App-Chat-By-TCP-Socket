#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deleteMemberOfGroup(int clientSocket)
{
    char group_id[20]; // Thay đổi kích thước tùy theo yêu cầu thực tế
    char user_id[20];  // Thay đổi kích thước tùy theo yêu cầu thực tế

    // Nhập group_id và user_id từ người dùng
    printf("Nhập groupid: ");
    scanf("%19s", group_id);
    printf("Nhập userid: ");
    scanf("%19s", user_id);

    // Tạo chuỗi remove_user_to_group
    char message[50]; // Thay đổi kích thước tùy theo yêu cầu thực tế
    snprintf(message, sizeof(message), "remove_user_to_group\n%s\n%s\r\n", group_id, user_id);

    // Gửi lệnh lên Server
    send(clientSocket, message, strlen(message), 0);

    // Nhận mã giao thức từ Server
    char protocolCode[5];
    recv(clientSocket, protocolCode, 10, 0);
    protocolCode[4] = '\0';
    printf("Protocol code: %s\n", protocolCode);
    // Xử lý mã giao thức từ Server
    if (strcmp(protocolCode, "2006") == 0)
    {
        printf("remove_member_to_group_success\n");
    }
    else if (strcmp(protocolCode, "4106") == 0)
    {
        printf("remove_member_to_group_not_success: Không tồn tại user id\n");
    }
    else if (strcmp(protocolCode, "4206") == 0)
    {
        printf("remove_member_to_group_not_success: Không tồn tại Group id\n");
    }
    else if (strcmp(protocolCode, "4306") == 0)
    {
        printf("remove_member_to_group_not_success: Lỗi mở file trên Server\n");
    }
    else if (strcmp(protocolCode, "4406") == 0)
    {
        printf("remove_member_to_group_not_success: Người dùng yêu cầu xóa chưa tham gia nhóm\n");
    }
    else if (strcmp(protocolCode, "4506") == 0)
    {
        printf("remove_member_to_group_not_success: Bạn không có quyền! (Vì bạn chưa tham gia nhóm này)\n");
    }
    else if (strcmp(protocolCode, "4606") == 0)
    {
        printf("remove_member_to_group_not_success:Bạn chưa đăng nhập\n");
    }
    else
    {
        printf("Mã giao thức không xác định: %s\n", protocolCode);
    }
}