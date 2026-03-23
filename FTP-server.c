#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF 1024

int main() {
    int srv, cli;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char filename[BUF], buffer[BUF];

    srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(srv, (struct sockaddr *)&addr, sizeof(addr));
    listen(srv, 5);
    printf("Server listening on port %d...\n", PORT);

    while (1) {
        cli = accept(srv, (struct sockaddr *)&addr, &len);
        printf("Client connected.\n");

        int n = recv(cli, filename, BUF - 1, 0);
        filename[n] = '\0';
        printf("Requested: %s\n", filename);

        FILE *f = fopen(filename, "rb");  // rb = works for ALL file types
        if (!f) {
            send(cli, "ERROR: File Not Found", 21, 0);
            printf("File not found.\n");
        } else {
            while ((n = fread(buffer, 1, BUF, f)) > 0)
                send(cli, buffer, n, 0);
            fclose(f);
            printf("File sent.\n");
        }
        close(cli);
    }
}
