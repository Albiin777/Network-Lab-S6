#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

static const char SERVER_ERR_MSG[] = "ERROR: File Not Found";

static int send_all(int sock, const char *buf, int len) {
    int sent = 0;
    while (sent < len) {
        int n = send(sock, buf + sent, len - sent, 0);
        if (n <= 0) {
            return -1;
        }
        sent += n;
    }
    return 0;
}

int handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char filename[BUFFER_SIZE] = {0};

    int bytes_received = recv(client_socket, filename, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        return 0;
    }
    filename[bytes_received] = '\0'; 

    if (strcmp(filename, "exit") == 0) {
        close(client_socket);
        return 1;
    }

    printf("Client requested: %s\n", filename);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("File not found: %s\n", filename);
        send_all(client_socket, SERVER_ERR_MSG, (int)strlen(SERVER_ERR_MSG));
    } else {
        size_t n;
        while ((n = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            if (send_all(client_socket, buffer, (int)n) == -1) {
                break;
            }
        }
        printf("File sent successfully.\n");
        fclose(file);
    }

    close(client_socket);
    return 0;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        close(server_socket);
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Client connected.\n");
        if (handle_client(client_socket)) {
            break;
        }
    }

    close(server_socket);
    return 0;
}
