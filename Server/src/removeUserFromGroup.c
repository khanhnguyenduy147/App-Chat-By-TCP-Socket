#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sessionManager.h"
#include "../lib/log.h"
#define MAX_LINE_LENGTH 256

void removeUserFromGroup(char *message, int socket_fd, const struct Session **sessionList)
{
    char group_id[20]; // Thay đổi kích thước tùy theo yêu cầu thực tế
    char user_id[20];  // Thay đổi kích thước tùy theo yêu cầu thực tế

    // Tách group_id và user_id từ message
    char *token = strtok(message, "\n");
    if (token != NULL)
    {
        strncpy(group_id, token, sizeof(group_id) - 1);
        group_id[sizeof(group_id) - 1] = '\0';

        token = strtok(NULL, "\n");
        if (token != NULL)
        {
            strncpy(user_id, token, sizeof(user_id) - 1);
            user_id[sizeof(user_id) - 1] = '\0';
        }
        else
        {
            // Xử lý lỗi khi không có user_id
            logResponse("4706", socket_fd);
            send(socket_fd, "4706", sizeof("4706"), 0);
            return;
        }
    }
    else
    {
        // Xử lý lỗi khi không có group_id
        logResponse("4706", socket_fd);
        send(socket_fd, "4706", sizeof("4706"), 0);
        return;
    }
    // Kiểm tra tài user id hợp lệ không
    FILE *accountFile = fopen("../database/account.txt", "r");
    if (accountFile == NULL)
    {
        perror("Error opening file");
        logResponse("4306", socket_fd);
        send(socket_fd, "4306", sizeof("4306"), 0);
        return;
    }
    if (!isUserId1(user_id, accountFile))
    {
        logResponse("4106", socket_fd);
        send(socket_fd, "4106", sizeof("4106"), 0);
    }
    char *user_id_self = findUserIdBySocket(*sessionList, socket_fd);
    // Kiểm tra đăng nhập hay chưa
    if (user_id_self == NULL)
    {
        // Chưa đăng nhập
        logResponse("4506", socket_fd);
        send(socket_fd, "4506", sizeof("4506"), 0);
        return;
    }
    // Mở file để đọc và ghi
    FILE *file = fopen("../database/group.txt", "r+");
    if (file == NULL)
    {
        // Xử lý lỗi khi không mở được file
        perror("Error opening group.txt");
        logResponse("4306", socket_fd);
        send(socket_fd, "4306", sizeof("4306"), 0);
        return;
    }
    // Kiểm tra xem group_id có tồn tại không
    if (!isGroupExist(group_id, file))
    {
        fclose(file);
        logResponse("4206", socket_fd);
        send(socket_fd, "4206", sizeof("4206"), 0);
        return;
    }
    // Kiểm tra bản thân đã tham gia vào nhóm chưa
    if (!checkJoinGroup(user_id_self, group_id, file))
    {
        logResponse("4506", socket_fd);
        send(socket_fd, "4506", sizeof("4506"), 0);
        return;
    }

    // Tạo buffer để đọc dữ liệu từ file
    char buffer[1024]; // Thay đổi kích thước tùy theo yêu cầu thực tế

    // Tìm và xóa user_id khỏi nhóm có group_id
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char read_group_id[20];
        char read_user_id[20];

        if (sscanf(buffer, "%19s %19s", read_group_id, read_user_id) == 2)
        {
            if (strcmp(read_group_id, group_id) == 0 && strcmp(read_user_id, user_id) == 0)
            {
                found = 1;
                break;
            }
        }
    }

    // Đặt con trỏ tập tin về đầu file để xóa dòng
    rewind(file);

    if (found)
    {
        FILE *temp_file = fopen("../database/temp_group.txt", "w");
        if (temp_file == NULL)
        {
            // Xử lý lỗi khi không tạo được tập tin tạm thời
            perror("Error creating temp_group.txt");
            logResponse("4306", socket_fd);
            send(socket_fd, "4306", sizeof("4306"), 0);
            fclose(file);
            return;
        }

        // Copy nội dung từ group.txt sang temp_group.txt, không bao gồm dòng chứa user_id
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            char read_user_id[20];
            if (sscanf(buffer, "%*s %19s", read_user_id) != 1 || strcmp(read_user_id, user_id) != 0)
            {
                fputs(buffer, temp_file);
            }
        }

        // Đóng tập tin temp_group.txt và group.txt
        fclose(temp_file);
        fclose(file);

        // Xóa group.txt và đổi tên temp_group.txt thành group.txt
        remove("../database/group.txt");
        rename("../database/temp_group.txt", "../database/group.txt");

        // Gửi mã giao thức thành công về cho client
        logResponse("2006", socket_fd);
        send(socket_fd, "2006", sizeof("2006"), 0);
    }
    else
    {
        // Gửi mã giao thức không thành công về cho client vì không tìm thấy user_id trong nhóm
        logResponse("4406", socket_fd);
        send(socket_fd, "4406", sizeof("4406"), 0);
        fclose(file);
    }
}
int isUserId1(const char *user_id, FILE *accountFile)
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
int checkJoinGroup(char *user_id_check, char *group_id_check, FILE *file)
{
    rewind(file);
    int join = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char read_group_id[20];
        char read_user_id[20];

        if (sscanf(buffer, "%19s %19s", read_group_id, read_user_id) == 2)
        {
            if (strcmp(read_group_id, group_id_check) == 0 && strcmp(read_user_id, user_id_check) == 0)
            {
                join = 1;
                break;
            }
        }
    }

    // Đặt con trỏ tập tin về đầu file để xóa dòng
    rewind(file);
    return join;
}