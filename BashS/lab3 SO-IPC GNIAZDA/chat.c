#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int listenfd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];

    fd_set active_fds, read_fds;

    // Mapping between client IDs and file descriptors
    int client_fds[MAX_CLIENTS] = {0};

    FD_ZERO(&active_fds);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, MAX_CLIENTS) == -1) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    FD_SET(listenfd, &active_fds);

    while (1) {
        read_fds = active_fds;

        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listenfd) {
                    // New connection
                    client_addr_len = sizeof(client_addr);
                    connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
                    if (connfd == -1) {
                        perror("accept()");
                        exit(EXIT_FAILURE);
                    }

                    fprintf(stdout, "New connection from %s:%d\n",
                            inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                    // Assign a client ID to the new connection
                    int client_id;
                    for (client_id = 0; client_id < MAX_CLIENTS; ++client_id) {
                        if (client_fds[client_id] == 0) {       // Check if occurred already
                            client_fds[client_id] = connfd;
                            fprintf(stdout, "Assigned client ID %d to new connection\n", client_id);
                            break;
                        }
                    }

                    FD_SET(connfd, &active_fds);
                } else {
                    // Existing client's data
                    int nbytes = read(i, buffer, sizeof(buffer));
                    int client_id;
                    for (client_id = 0; client_id < MAX_CLIENTS; ++client_id) {
                        if (client_fds[client_id] == i) {       // Corelate FD with client_fds array
                            break;
                        }
                    }

                    if (nbytes <= 0) {
                        // Connection closed or error
                        fprintf(stdout, "Connection closed by client %d\n", client_id);
                        close(i);
                        FD_CLR(i, &active_fds);
                        client_fds[client_id] = 0;
                    } else {
                        // Broadcast message to all clients with information about the sender
                        for (int j = 0; j < FD_SETSIZE; ++j) {
                            if (FD_ISSET(j, &active_fds) && j != listenfd && j != i) {
                                struct sockaddr_in sender_addr;
                                socklen_t sender_addr_len = sizeof(sender_addr);
                                getpeername(i, (struct sockaddr*)&sender_addr, &sender_addr_len);
                                dprintf(j, "From %s:%d (client %d): %.*s",
                                        inet_ntoa(sender_addr.sin_addr),
                                        ntohs(sender_addr.sin_port),
                                        client_id,
                                        nbytes, buffer);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
