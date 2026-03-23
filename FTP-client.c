#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUF 1024

void get_unique_filename(char *filename) {
    if (access(filename, F_OK) != 0) return;  // doesn't exist, use as-is

    char base[BUF], ext[BUF], newname[BUF];
    char *dot = strrchr(filename, '.');

    if (dot) {
        int baselen = dot - filename;
        strncpy(base, filename, baselen);
        base[baselen] = '\0';
        strcpy(ext, dot);              // ext = ".txt", ".pdf", etc.
    } else {
        strcpy(base, filename);
        ext[0] = '\0';                 // no extension
    }

    int counter = 1;
    do {
        snprintf(newname, BUF, "%s(%d)%s", base, counter++, ext);
    } while (access(newname, F_OK) == 0);  // try (1), (2), (3)...

    strcpy(filename, newname);
}

int main() {
    int sock;
    struct sockaddr_in addr;
    char filename[BUF], buffer[BUF], savename[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connected. Enter filename: ");
    scanf("%1023s", filename);

    send(sock, filename, strlen(filename), 0);

    strcpy(savename, filename);
    get_unique_filename(savename);      // rename if file already exists

    int n = recv(sock, buffer, BUF, 0);
    if (n <= 0) { printf("No data received.\n"); close(sock); return 1; }

    if (strncmp(buffer, "ERROR: File Not Found", 21) == 0) {
        printf("Server: File not found.\n");
        close(sock);
        return 1;
    }

    FILE *f = fopen(savename, "wb");   // wb = works for ALL file types
    int total = 0;
    fwrite(buffer, 1, n, f);
    total += n;
    while ((n = recv(sock, buffer, BUF, 0)) > 0) {
        fwrite(buffer, 1, n, f);
        total += n;
    }
    fclose(f);
    close(sock);

    printf("Saved as: %s (%d bytes)\n", savename, total);
    return 0;
}
