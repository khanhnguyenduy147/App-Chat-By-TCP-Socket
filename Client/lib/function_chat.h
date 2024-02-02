#ifndef FUNCTIONCHAT_H
#define FUNCTIONCHAT_H
void addMembersToGroupFunction(int clientSocket);
void chatBetweenTwoPeopleFunction(int clientSocket);
void chatGroup(int clientSocket);
void CreateChatGroupFunction(int clientSocket);
void deleteMemberOfGroup(int clientSocket);
void friendRequestNotificationFunction(int clientSocket);
void leaveGroup(int clientSocket);
int loginFunction(int clientSocket);
void registrationFunction(int clientSocket);
int sendFriendRequestFunction(int clientSocket);
void viewFriendsListFunction(int clientSocket);
void chatGroup(int clientSocket);
void getContentGroup(char *idgroup, int clientSocket);
void disconnectFunction(int clientSocket);
#endif
