#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void getCurrentTime(char *timeStr)
{
    time_t rawtime;
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(timeStr, 20, "%H:%M:%S-%d:%m:%Y", info);
}

void logRequest(const char *messRequest, int clientSocket)
{
    FILE *logFile = fopen("log_server.txt", "a");
    if (logFile == NULL)
    {
        perror("Error opening log_server.txt");
        return;
    }

    char timeStr[20];
    getCurrentTime(timeStr);

    char clientAddress[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(clientSocket, (struct sockaddr *)&addr, &addr_len);
    inet_ntop(AF_INET, &(addr.sin_addr), clientAddress, INET_ADDRSTRLEN);

    char processedMessRequest[2 * strlen(messRequest) + 1];
    for (int i = 0, j = 0; messRequest[i] != '\0'; i++, j++)
    {
        if (messRequest[i] == '\n')
        {
            processedMessRequest[j++] = '\\';
            processedMessRequest[j] = 'n';
        }
        else if (messRequest[i] == '\r')
        {
            processedMessRequest[j++] = '\\';
            processedMessRequest[j] = 'r';
        }
        else
        {
            processedMessRequest[j] = messRequest[i];
        }
    }
    processedMessRequest[2 * strlen(messRequest)] = '\0';

    fprintf(logFile, "%s$%s$%d$%s$", timeStr, clientAddress, ntohs(addr.sin_port), processedMessRequest);

    fclose(logFile);
}

void logResponse(const char *messResponse, int clientSocket)
{
    FILE *logFile = fopen("log_server.txt", "a");
    if (logFile == NULL)
    {
        perror("Error opening log_server.txt");
        return;
    }

    char processedMessResponse[2 * strlen(messResponse) + 2]; // 2 để thêm dấu + hoặc -
    int isPlusSign = 0;

    // Xác định nếu ký tự đầu là 2 hoặc 4
    if (messResponse[0] == '2')
    {
        isPlusSign = 1;
    }
    else if (messResponse[0] == '4')
    {
        isPlusSign = 0;
    }

    // Chèn dấu + hoặc - vào processedMessResponse
    processedMessResponse[0] = (isPlusSign) ? '+' : '-';
    for (int i = 0, j = 1; messResponse[i] != '\0'; i++, j++)
    {
        if (messResponse[i] == '\n')
        {
            processedMessResponse[j++] = '\\';
            processedMessResponse[j] = 'n';
        }
        else if (messResponse[i] == '\r')
        {
            processedMessResponse[j++] = '\\';
            processedMessResponse[j] = 'r';
        }
        else
        {
            processedMessResponse[j] = messResponse[i];
        }
    }
    processedMessResponse[2 * strlen(messResponse) + 1] = '\0';

    fprintf(logFile, "%s$", processedMessResponse);

    fprintf(logFile, "\n");

    fclose(logFile);
}
