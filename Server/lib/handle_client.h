#include <stdio.h>
#include "../lib/sessionManager.h"
#define BUFF_SIZE 2048

#ifndef HANDLECLIENT_H
#define HANDLECLIENT_H

// Structure to hold thread arguments
struct ThreadArgs
{
    int *gen_id;
    int conn_fd;                         // Socket identifier for the client connection
    FILE *file;                          // File to be passed to the function for processing
    struct sockaddr_in client_sock_addr; // Add client variable to the structure
    struct Session **sessionList;        // Add sessionList variable to the structure
};

/**
 * handleClient - Function to handle client requests in a separate thread.
 *
 * This function handles incoming client requests in a separate thread, processing various commands and interacting with a session management system.
 *
 * @param arg               A pointer to a structure containing thread arguments.
 * @return                  NULL.
 */
void *handleClient(void *arg);

#endif