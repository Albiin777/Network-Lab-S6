#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

int main(void) {
    int port;

    printf("Enter port number: ");
    if (scanf("%d", &port) != 1 || port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        exit(EXIT_FAILURE);
    }

    while (getchar() != '\n' && !feof(stdin)) {
    }

    int sockfd, newsockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d... (Waiting for client)\n", port);

    addr_size = sizeof(client_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);
    if (newsockfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    while (1) {
        int bytes_read = recv(newsockfd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_read <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_read] = '\0';

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client asked to quit.\n");
            break;
        }

        printf("Client: %s", buffer);

        printf("Server: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Server exiting...\n");
            break;
        }

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server exiting...\n");
            break; 
        }
        send(newsockfd, buffer, strlen(buffer), 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

