#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../lib/sessionManager.h"


void processRemoveFriend(char *message, int socket_fd) {
    char userId1[256];
    char userId2[256];
        if (sscanf(message, "\n%s\n%s\n\r",  userId1, userId2) == 2) {
          
                 FILE *file = fopen("friend.txt", "r");
                if (file == NULL) {
                perror("Error opening file");
                exit(1);
                }
                int found = 0;
                char tempUserId1[256];
                char tempUserId2[256];
                while (fscanf(file, "%s %s", tempUserId1, tempUserId2) != EOF) {
                if ((strcmp(userId1, tempUserId1) == 0 && strcmp(userId2, tempUserId2) == 0) ||
                    (strcmp(userId2, tempUserId1) == 0 && strcmp(userId1, tempUserId2) == 0)) {
                    found = 1; 
                } 
                }
                  fclose(file);
             if (found) {
                //Huỷ kết bạn thành công
                send(socket_fd, "2009",sizeof("2009"), 0);
             }else{
                // Không tìm thấy ID, gửi response 4009
                send(socket_fd, "4009", sizeof("4009"), 0);
             }
            
        }
}