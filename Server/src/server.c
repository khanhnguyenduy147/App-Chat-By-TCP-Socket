#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../lib/sessionManager.h"
#include "../lib/handle_client.h"

#define BACKLOG 20

// Global session list
struct Session *sessionList = NULL;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Port_Number>\n", argv[0]);
        return 1;
    }

    int listen_fd, conn_fd, gen_id = 0;
    struct sockaddr_in server, client;
    socklen_t client_len;

    // Create a socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    // Initialize server structure
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the server address
    if (bind(listen_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("bind");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(listen_fd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    // Open the account file for reading
    FILE *file = fopen("../database/account.txt", "r+");
    if (file == NULL)
    {
        perror("Cannot open file");
        return 0; // Error opening file
    }

    pthread_t tid;

    while (1)
    {
        client_len = sizeof(client);

        // Accept incoming connection
        if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client, &client_len)) == -1)
        {
            perror("accept");
            continue;
        }

        struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
        args->gen_id = &gen_id;
        args->conn_fd = conn_fd;
        args->file = file;
        args->client_sock_addr = client;
        args->sessionList = &sessionList;

        pthread_create(&tid, NULL, &handleClient, (void *)args);
        // Detach the thread to automatically clean up resources
        pthread_detach(tid);
    }

    // Free memory when the program ends
    struct Session *currentSession = sessionList;
    while (currentSession != NULL)
    {
        struct Session *nextSession = currentSession->next;
        free(currentSession->userId);
        free(currentSession);
        currentSession = nextSession;
    }

    fclose(file);
    close(listen_fd);
    return 0;
}
