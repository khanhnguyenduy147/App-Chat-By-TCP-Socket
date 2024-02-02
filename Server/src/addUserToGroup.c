#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"
#include "../lib/log.h"
#define MAX_LINE_LENGTH 256

void addUserToGroup(char *message, int socket_fd, const struct Session **sessionList)
{
    char group_id[20]; // Thay đổi kích thước tùy theo yêu cầu thực tế
    char user_id[20];  // Thay đổi kích thước tùy theo yêu cầu thực tế

    // Tách group_id và user_id từ message
    if (sscanf(message, "%19s\n%19s", group_id, user_id) != 2)
    {
        // Phân tích không thành công
        logResponse("4605", socket_fd);
        send(socket_fd, "4605", sizeof("4605"), 0);
        return;
    }
    // Kiểm tra tài user id hợp lệ không
    FILE *accountFile = fopen("../database/account.txt", "r");
    if (accountFile == NULL)
    {
        perror("Error opening file");
        logResponse("4605", socket_fd);
        send(socket_fd, "4605", sizeof("4605"), 0);
        return;
    }
    if (!isUserId(user_id, accountFile))
    {
        logResponse("4405", socket_fd);
        send(socket_fd, "4405", sizeof("4405"), 0);
    }
    char *user_id_self = findUserIdBySocket(*sessionList, socket_fd);
    // Kiểm tra đăng nhập hay chưa
    if (user_id_self == NULL)
    {
        // Chưa đăng nhập
        logResponse("4505", socket_fd);
        send(socket_fd, "4505", sizeof("4505"), 0);
        return;
    }
    // Mở file group.txt để kiểm tra và thêm user vào nhóm
    FILE *groupFile = fopen("../database/group.txt", "a+");
    if (!groupFile)
    {
        perror("Error opening group.txt");
        logResponse("4605", socket_fd);
        send(socket_fd, "4605", sizeof("4605"), 0);
        return;
    }

    // Kiểm tra xem user_id có tồn tại trong friend_list không
    FILE *friendFile = fopen("../database/friend.txt", "r");
    if (friendFile == NULL)
    {
        perror("Error opening file");
        logResponse("4605", socket_fd);
        send(socket_fd, "4605", sizeof("4605"), 0);
        return;
    }
    if (!isUserInFriendList(user_id, user_id_self))
    {
        fclose(groupFile);
        logResponse("4105", socket_fd);
        send(socket_fd, "4105", sizeof("4105"), 0);
        return;
    }

    // Kiểm tra xem group_id có tồn tại không
    if (!isGroupExist(group_id, groupFile))
    {
        fclose(groupFile);
        logResponse("4205", socket_fd);
        send(socket_fd, "4205", sizeof("4205"), 0);
        return;
    }

    // Kiểm tra xem user_id đã tồn tại trong nhóm chưa
    if (isUserInGroup(group_id, user_id, groupFile))
    {
        fclose(groupFile);
        logResponse("4305", socket_fd);
        send(socket_fd, "4305", sizeof("4305"), 0);
        return;
    }

    // Thêm user_id vào nhóm trong file
    fprintf(groupFile, "\n%s %s", group_id, user_id);
    fclose(groupFile);
    logResponse("2005", socket_fd);
    send(socket_fd, "2005", sizeof("2005"), 0);
}

int isUserInFriendList(const char *user_id_friend, const char *user_id_self)
{
    printf("%s %s", user_id_friend, user_id_self);
    FILE *file = fopen("../database/friend.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char friend1[20], friend2[20];
        if (sscanf(line, "%19s %19s", friend1, friend2) == 2)
        {
            if ((strcmp(friend1, user_id_friend) == 0 && strcmp(friend2, user_id_self) == 0) ||
                (strcmp(friend1, user_id_self) == 0 && strcmp(friend2, user_id_friend) == 0))
            {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

int isGroupExist(const char *group_id, FILE *groupFile)
{
    // Cài đặt kiểm tra xem group_id có tồn tại không
    // Trả về 1 nếu tồn tại, 0 nếu không tồn tại
    // (Bạn cần cài đặt logic kiểm tra này)
    char line[40];
    fseek(groupFile, 0, SEEK_SET); // Di chuyển về đầu file
    while (fgets(line, sizeof(line), groupFile) != NULL)
    {
        char current_group_id[20];
        if (sscanf(line, "%19s", current_group_id) == 1)
        {
            if (strcmp(group_id, current_group_id) == 0)
            {
                return 1; // Tìm thấy group_id
            }
        }
    }
    return 0; // Không tìm thấy group_id
}

int isUserInGroup(const char *group_id, const char *user_id, FILE *groupFile)
{
    // Cài đặt kiểm tra xem user_id đã tồn tại trong nhóm chưa
    // Trả về 1 nếu tồn tại, 0 nếu không tồn tại
    // (Bạn cần cài đặt logic kiểm tra này)
    char line[40];
    fseek(groupFile, 0, SEEK_SET); // Di chuyển về đầu file
    while (fgets(line, sizeof(line), groupFile) != NULL)
    {
        char current_group_id[20], current_user_id[20];
        if (sscanf(line, "%19s %19s", current_group_id, current_user_id) == 2)
        {
            if (strcmp(group_id, current_group_id) == 0 && strcmp(user_id, current_user_id) == 0)
            {
                return 1; // Tìm thấy user_id trong nhóm
            }
        }
    }
    return 0; // Không tìm thấy user_id trong nhóm
}

int isUserId(const char *user_id, FILE *accountFile)
{
    int return_result = 0;
    char line[MAX_LINE_LENGTH];
    // Đọc từng dòng từ file
    while (fgets(line, sizeof(line), accountFile) != NULL)
    {
        char fullname[MAX_LINE_LENGTH];
        char fileUsername[MAX_LINE_LENGTH];
        char filePassword[MAX_LINE_LENGTH];
        char userId[MAX_LINE_LENGTH];
        int fileStatus;

        // Phân tích thông tin từ dòng
        if (sscanf(line, "%s %s %s %s %d", fullname, fileUsername, filePassword, userId, &fileStatus) != 5)
        {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }
        if (strcmp(user_id, userId) == 0)
        {
            return_result = 1;
        }
    }

    // Đóng file
    fclose(accountFile);
    return return_result;
}