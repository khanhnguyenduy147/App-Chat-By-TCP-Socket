#ifndef LOG_H
#define LOG_H
void logRequest(const char *request, int socket_fd);
void logResponse(const char *response, int socket_fd);
#endif
