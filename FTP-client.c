#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

static const char SERVER_ERR_MSG[] = "ERROR: File Not Found";

void get_unique_filename(char *filename) {
    if (access(filename, F_OK) == -1) return;

    char base[BUFFER_SIZE], ext[BUFFER_SIZE], new_name[BUFFER_SIZE];
    int counter = 1;
    char *dot = strrchr(filename, '.');

    if (dot) {
        size_t len = dot - filename;
        if (len >= sizeof(base)) len = sizeof(base) - 1;
        memcpy(base, filename, len);
        base[len] = '\0';
        strncpy(ext, dot, sizeof(ext) - 1);
        ext[sizeof(ext) - 1] = '\0';
    } else {
        strncpy(base, filename, sizeof(base) - 1);
        base[sizeof(base) - 1] = '\0';
        ext[0] = '\0';
    }

    do {
        char num_part[32];

        snprintf(num_part, sizeof(num_part), "_%d", counter++);
        new_name[0] = '\0';

        strncat(new_name, base, sizeof(new_name) - strlen(new_name) - 1);
        strncat(new_name, num_part, sizeof(new_name) - strlen(new_name) - 1);
        strncat(new_name, ext, sizeof(new_name) - strlen(new_name) - 1);
    } while (access(new_name, F_OK) != -1);
    strncpy(filename, new_name, BUFFER_SIZE - 1);
    filename[BUFFER_SIZE - 1] = '\0';
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], req_file[BUFFER_SIZE], final_name[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        close(sock);
        return 1;
    }

    printf("Connected to server.\n");
    printf("Enter filename: ");
    if (scanf("%1023s", req_file) != 1) {
        close(sock);
        return 1;
    }

    if (strcmp(req_file, "exit") == 0) {
        close(sock);
        return 0;
    }

    if (send(sock, req_file, strlen(req_file), 0) < 0) {
        close(sock);
        return 1;
    }

    strncpy(final_name, req_file, sizeof(final_name) - 1);
    final_name[sizeof(final_name) - 1] = '\0';
    get_unique_filename(final_name);

    FILE *file = fopen(final_name, "wb");
    if (file == NULL) {
        close(sock);
        return 1;
    }

    int n, total = 0, is_err = 0;

    while ((n = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        if (total == 0 && n == (int)strlen(SERVER_ERR_MSG) &&
            memcmp(buffer, SERVER_ERR_MSG, strlen(SERVER_ERR_MSG)) == 0) {
            is_err = 1;
            break;
        }
        fwrite(buffer, 1, n, file);
        total += n;
    }

    fclose(file);
    close(sock);

    if (is_err) {
        printf("[!] Server Error: File not found.\n");
        remove(final_name);
    } else if (total > 0) {
        printf("       FILE RECEIVED AT CLIENT      \n");
	printf("\n");
        printf("Saved as: %s (%d bytes)\n", final_name, total);

    } else {
        printf("[!] Received 0 bytes. Check if 'sample.txt' exists in server folder.\n");
        remove(final_name);
    }
    return 0;
}
