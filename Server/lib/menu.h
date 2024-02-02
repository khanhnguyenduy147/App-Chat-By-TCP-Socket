#ifndef MENU_H
#define MENU_H
void handleLogin(char *message, int socket_fd, const struct Session *sessionList);
void getFriendListAndStatus(char *user_id, int socket_fd, const struct Session *sessionList);
void getGroupListByUserId(char *user_id, int socket_fd);
void addUserToGroup(char *message, int socket_fd, const struct Session **sessionList);
void removeUserFromGroup(char *message, int socket_fd, const struct Session **sessionList);
void getContentGroup(char *user_id, int socket_fd, char *message);
void chatGroup(char *message, int socket_fd, char *user_id, const struct Session **sessionList);
void handleRegistration(char *message, int *gen_id, int socket_fd);
void friendRequest(char *message, int socket_fd, const struct Session *sessionList);
void disconnect(char *message, int socket_fd);
#endif
