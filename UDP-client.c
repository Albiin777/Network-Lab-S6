#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

int main() {
    int port;
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len;


    printf("Enter Server Port Number: ");
    scanf("%d", &port);
    getchar(); 

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    printf("UDP Client ready on port %d. Type 'exit' to quit.\n", port);

    while (1) {
        printf("Client: ");
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0, 
              (const struct sockaddr *)&server_addr, sizeof(server_addr));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting...\n");
            break;
        }

        addr_len = sizeof(server_addr);
        memset(buffer, 0, BUFFER_SIZE);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                        (struct sockaddr *)&server_addr, &addr_len);
        
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }

    close(sockfd);
    return 0;
}
