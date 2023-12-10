#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8082
#define MAX_BUFFER_SIZE 1024

void handle_get(int client_socket, const char *request);
void handle_post(int client_socket, const char *request);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("Error accepting connection");
            continue;
        }

        printf("Connection accepted from %s\n", inet_ntoa(client_addr.sin_addr));

        char request[MAX_BUFFER_SIZE];
        ssize_t bytes_received = recv(client_socket, request, sizeof(request), 0);
        if (bytes_received == -1) {
            perror("Error receiving request");
            close(client_socket);
            continue;
        }

        request[bytes_received] = '\0';

        if (strstr(request, "GET") != NULL) {
            handle_get(client_socket, request);
        } else if (strstr(request, "POST") != NULL) {
            handle_post(client_socket, request);
        }

        close(client_socket);
    }

    close(server_socket);

    return 0;
}

void handle_get(int client_socket, const char *request) {
    char file_path[MAX_BUFFER_SIZE];
    sscanf(request, "GET %s HTTP/1.1", file_path);

    if (strcmp(file_path, "/") == 0 || strcmp(file_path, "/index.html") == 0) {
        char response[MAX_BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        write(client_socket, response, strlen(response));

        char message[] = "<html><body><h1>This is System-Programming Lab4-7 Web by.TaeminKim</h1></body></html>";
        write(client_socket, message, strlen(message));
    } else if (strstr(file_path, "/cgi-bin/") != NULL) {
        // The rest of the CGI handling code...
        // (unchanged from the original code)
    } else {
        // The rest of the static files handling code...
        // (unchanged from the original code)
    }
}

void handle_post(int client_socket, const char *request) {
    char file_path[MAX_BUFFER_SIZE];
    sscanf(request, "POST %s HTTP/1.1", file_path);

    FILE *file = fopen(file_path + 1, "rb");
    if (file == NULL) {
        char response[MAX_BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 404 Not Found\r\n\r\n");
        write(client_socket, response, strlen(response));
    } else {
        char buffer[MAX_BUFFER_SIZE];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            write(client_socket, buffer, bytes_read);
        }
        fclose(file);
    }
}
