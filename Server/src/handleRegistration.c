#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LINE_LENGTH 256
struct UserInfo
{
    char fullname[256];
    char username[256];
    char password[256];
    int gen_id;
};
// Lưu thông tin người dùng
int save(char *fullname, char *username, char *passsword, int *gen_id);
// Kiểm tra người dùng có tồn tại hay không
int check(char *username);
// Lưu giá trị gen_id vào tệp
void saveGenId(int gen_id);
int readGenId();
// Xử lý đăng ký người dùng
// char *genId(int gen_code);
void handleRegistration(char *message, int *gen_id, int socket_fd)
{

    char fullname[256];
    char username[256];
    char password[256];

    if (sscanf(message, "%s\n%s\n%s\r\n", fullname, username, password) == 3)
    {

        *gen_id = readGenId();
        // Gọi
        if (check(username))
        {
            save(fullname, username, password, gen_id);

            send(socket_fd, "2001", sizeof("2001"), 0);
        }
        else
        {
            send(socket_fd, "4001", sizeof("4001"), 0);
        }
    }
}

int save(char *fullname, char *username, char *password, int *gen_id)
{
    // Mở file cơ sở dữ liệu để ghi
    FILE *dbFile = fopen("../database/account.txt", "a");
    if (dbFile == NULL)
    {
        perror("Error opening database file");
        return -1;
    }

    (*gen_id)++;

    fprintf(dbFile, "%s %s %s %d  1\n", fullname, username, password, *gen_id);
    // Lưu giá trị gen_id vào tệp
    saveGenId(*gen_id);
    fclose(dbFile);
    return 0;
}
int check(char *username)
{
    FILE *file = fopen("../database/account.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    // Khai báo biến tạm thời để lưu thông tin người dùng từ file

    char fullname[MAX_LINE_LENGTH];
    char fileUsername[MAX_LINE_LENGTH];
    char filePassword[MAX_LINE_LENGTH];
    char userId[MAX_LINE_LENGTH];
    int fileStatus;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL)
    {

        // So sánh username của người dùng với username trong file
        if (sscanf(line, "%s %s %s %s %d", fullname, fileUsername, filePassword, userId, &fileStatus) != 5)
        {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }
        if (strcmp(username, fileUsername) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    fclose(file);
    return 0;
}
// Lưu giá trị gen_id vào tệp
void saveGenId(int gen_id)
{
    FILE *genId = fopen("../database/gen_id.txt", "w"); // Chế độ w trong fopen sẽ ghi đè
    if (genId != NULL)
    {
        fprintf(genId, "%d", gen_id);
        fclose(genId);
    }
}
// Đọc giá trị gen_id từ tệp
int readGenId()
{
    int gen_id;
    FILE *genId = fopen("../database/gen_id.txt", "r");
    if (genId != NULL)
    {
        fscanf(genId, "%d", &gen_id);
        fclose(genId);
    }
    return gen_id;
}
