#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/log.h"
#include "../lib/sessionManager.h"

#define MAX_LINE_LENGTH 256
#define MAX_MESSAGE_LENGTH 1024

void chatGroup(char *message, int socket_fd, char *user_id, const struct Session **sessionList)
{
    // Nhận thông tin từ chuỗi message
    char groupid[MAX_LINE_LENGTH];
    char content[MAX_MESSAGE_LENGTH];
    sscanf(message, "%s\n%[^\n]", groupid, content);

    // Tạo thời gian
    time_t current_time;
    char formatted_time[MAX_LINE_LENGTH];
    time(&current_time);
    strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S-%d:%m:%Y", localtime(&current_time));

    char *user_id_self = findUserIdBySocket(*sessionList, socket_fd);
    // Kiểm tra đăng nhập hay chưa
    if (user_id_self == NULL)
    {
        // Chưa đăng nhập
        logResponse("4207", socket_fd);
        send(socket_fd, "4207", sizeof("4207"), 0);
        return;
    }
    // Mở file group.txt để kiểm tra
    FILE *groupFile = fopen("../database/group.txt", "a+");
    if (!groupFile)
    {
        perror("Error opening group.txt");
        logResponse("4307", socket_fd);
        send(socket_fd, "4307", sizeof("4307"), 0);
        return;
    }
    // Kiểm tra xem group_id có tồn tại không
    if (!isGroupExist(groupid, groupFile))
    {
        fclose(groupFile);
        logResponse("4007", socket_fd);
        send(socket_fd, "4007", sizeof("4007"), 0);
        return;
    }
    // Kiểm tra bản thân đã tham gia vào nhóm chưa
    if (!checkJoinGroup(user_id_self, groupid, groupFile))
    {
        logResponse("4107", socket_fd);
        send(socket_fd, "4107", sizeof("4107"), 0);
        return;
    }
    // Ghi vào file
    char filename[MAX_LINE_LENGTH];
    snprintf(filename, sizeof(filename), "../database/message_group/%s.txt", groupid);

    FILE *file = fopen(filename, "a");
    if (!file)
    {
        // Gửi mã giao thức 4007 nếu không mở được file
        logResponse("4007", socket_fd);
        send(socket_fd, "4007", sizeof("4007"), 0);
        return;
    }

    fprintf(file, "%s$%s$%s\n", user_id, formatted_time, content);
    fclose(file);

    // Gửi mã giao thức 2007 nếu thành công
    logResponse("2007", socket_fd);
    send(socket_fd, "2007", sizeof("2007"), 0);
}
