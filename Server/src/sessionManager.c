#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "../lib/sessionManager.h"
static pthread_mutex_t sessionListMutex = PTHREAD_MUTEX_INITIALIZER;
// Hàm thêm một phiên mới vào danh sách
void addSession(struct Session **sessionList, int socketId, const char *userId)
{
    pthread_mutex_lock(&sessionListMutex);
    struct Session *newSession = (struct Session *)malloc(sizeof(struct Session));

    // Gán giá trị cho các trường trong cấu trúc
    newSession->socketId = socketId;

    // Cấp phát động cho username và sao chép giá trị vào đó
    newSession->userId = strdup(userId);

    newSession->next = *sessionList;

    // Cập nhật con trỏ đầu danh sách
    *sessionList = newSession;
    pthread_mutex_unlock(&sessionListMutex);
}

// Hàm kiểm tra xem một tài khoản có đang đăng nhập từ một client khác hay không
int isUserIdLoggedIn(const struct Session *sessionList, const char *userId)
{
    pthread_mutex_lock(&sessionListMutex);
    const struct Session *current = sessionList;
    while (current != NULL)
    {
        if (strcmp(current->userId, userId) == 0)
        {
            return current->socketId;
        }
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy
    pthread_mutex_unlock(&sessionListMutex);

    // Tài khoản không đang đăng nhập ở bất kỳ client nào khác
    return 0;
}

// Hàm xóa một phiên khỏi danh sách khi client đăng xuất
void removeSessionBySocket(struct Session **sessionList, int socketId)
{
    pthread_mutex_lock(&sessionListMutex);
    struct Session *currentSession = *sessionList;
    struct Session *prevSession = NULL;

    while (currentSession != NULL)
    {
        if (currentSession->socketId == socketId)
        {
            // Tìm thấy phiên cần xóa
            if (prevSession != NULL)
            {
                // Phiên không phải là nút đầu danh sách
                prevSession->next = currentSession->next;
            }
            else
            {
                // Phiên là nút đầu danh sách
                *sessionList = currentSession->next;
            }

            // Giải phóng bộ nhớ của username và sau đó giải phóng bộ nhớ của phiên bị xóa
            free(currentSession->userId);
            free(currentSession);
            break;
        }

        prevSession = currentSession;
        currentSession = currentSession->next;
    }
    pthread_mutex_unlock(&sessionListMutex);
}

// Hàm xóa một phiên khỏi danh sách khi client đăng xuất
void removeSessionByUserId(struct Session **sessionList, int userId)
{
    pthread_mutex_lock(&sessionListMutex);
    struct Session *currentSession = *sessionList;
    struct Session *prevSession = NULL;

    while (currentSession != NULL)
    {
        if (currentSession->userId == userId)
        {
            // Tìm thấy phiên cần xóa
            if (prevSession != NULL)
            {
                // Phiên không phải là nút đầu danh sách
                prevSession->next = currentSession->next;
            }
            else
            {
                // Phiên là nút đầu danh sách
                *sessionList = currentSession->next;
            }

            // Giải phóng bộ nhớ của username và sau đó giải phóng bộ nhớ của phiên bị xóa
            free(currentSession->userId);
            free(currentSession);
            break;
        }

        prevSession = currentSession;
        currentSession = currentSession->next;
    }
    pthread_mutex_unlock(&sessionListMutex);
}

int findSocketIdByUserId(const struct Session *sessionList, const char *userId)
{
    const struct Session *current = sessionList;
    while (current != NULL)
    {
        if (strcmp(current->userId, userId) == 0)
        {
            return current->socketId;
        }
        current = current->next;
    }
    return -1; // Trả về -1 nếu không tìm thấy
}

char *findUserIdBySocket(const struct Session *sessionList, const int socket_fd)
{
    const struct Session *current = sessionList;
    while (current != NULL)
    {
        if (current->socketId == socket_fd)
        {
            return current->userId;
        }
        current = current->next;
    }
    return NULL; // Trả về -1 nếu không tìm thấy
    printf("No finded!\n");
}