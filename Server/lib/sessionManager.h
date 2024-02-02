#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

// Cấu trúc lưu trữ thông tin của mỗi phiên
struct Session
{
    int socketId; // Định danh socket trên server
    char *userId;
    struct Session *next; // Con trỏ đến phiên tiếp theo trong danh sách liên kết
};

void addSession(struct Session **sessionList, int socketId, const char *userId);
int isUserIdLoggedIn(const struct Session *sessionList, const char *userId);
void removeSessionBySocket(struct Session **sessionList, int socketId);
void removeSessionByUserId(struct Session **sessionList, int userId);
int findSocketIdByUserId(const struct Session *sessionList, const char *userId);
char *findUserIdBySocket(const struct Session *sessionList, const int socket_fd);

#endif