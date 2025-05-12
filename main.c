#include "host.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void clear_stdin() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void server_handler(int cli_sock) {
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = read(cli_sock, buffer, BUFFER_SIZE);
        if (bytes <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;
        printf("Client says: %s\n", buffer);

        if (!strcmp(buffer, "exit")) {
            printf("Client requested exit.\n");
            break;
        }

        printf("Reply to client: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(cli_sock, message, strlen(message), 0);
    }

    close(cli_sock);
}

void client_handler(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};

    while (1) {
        printf("Message to server: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sock, message, strlen(message), 0);

        if (!strncmp(message, "exit", 4)) {
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes = read(sock, buffer, BUFFER_SIZE);
        if (bytes <= 0) {
            printf("Server closed connection.\n");
            break;
        }

        printf("Server says: %s\n", buffer);
    }
}

int main() {
    int choice = 0;
    int exit_flag = 0;

    while (!exit_flag) {
        printf("==== Host Menu ====\n");
        printf("1. Host a game\n");
        printf("2. Join a game\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clear_stdin();

        switch (choice) {
            case 1:
                start_server(server_handler);
                break;
            case 2: {
                char ip[16];
                printf("Enter server IP: ");
                scanf("%15s", ip);
                clear_stdin();
                start_client(ip, client_handler);
                break;
            }
            case 3:
                exit_flag = 1;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
