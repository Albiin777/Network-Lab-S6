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

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on port %d. Type 'exit' to quit.\n", port);

    while (1) {
        printf("Client: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Exiting...\n");
            break;
        }

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting...\n");
            break;
        }

        send(sockfd, buffer, strlen(buffer), 0);

        int bytes_read = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_read] = '\0';

        printf("Server: %s", buffer);
    }

    close(sockfd);
    return 0;
}
