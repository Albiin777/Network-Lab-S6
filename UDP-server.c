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
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    printf("Enter Port Number to listen on: ");
    scanf("%d", &port);
    getchar(); 


    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);


    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", port);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        addr_len = sizeof(client_addr);


        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                        (struct sockaddr *)&client_addr, &addr_len);
        
        buffer[n] = '\0';


        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client [%s:%d] disconnected.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            printf("Waiting for new client...\n");
            continue; 
        }

        printf("Client: %s", buffer);


        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0, 
              (struct sockaddr *)&client_addr, addr_len);
              
        // Optional: Server admin override to stop everything
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server shutting down...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
