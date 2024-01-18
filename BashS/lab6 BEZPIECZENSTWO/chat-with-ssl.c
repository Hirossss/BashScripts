#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define MAX_CLIENTS 12
#define BUFFER_SIZE 1024
#define USERNAME_SIZE 32

char client_usernames[MAX_CLIENTS][USERNAME_SIZE] = {0};

SSL_CTX* init_server_ctx(void) {
    SSL_CTX *ctx;

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

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

    int client_fds[MAX_CLIENTS] = {0};

    SSL_CTX *ctx;

    FD_ZERO(&active_fds);

    SSL_library_init();
    ctx = init_server_ctx();

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
                        if (client_fds[client_id] == 0) {
                            client_fds[client_id] = connfd;

                            // Initialize SSL
                            SSL *ssl = SSL_new(ctx);
                            SSL_set_fd(ssl, connfd);
                            if (SSL_accept(ssl) <= 0) {
                                ERR_print_errors_fp(stderr);
                                close(connfd);
                                FD_CLR(connfd, &active_fds);
                                client_fds[client_id] = 0;
                                continue;
                            }

                            SSL_write(ssl, "Enter your username: ", strlen("Enter your username: "));
                            ssize_t username_length = SSL_read(ssl, client_usernames[client_id], USERNAME_SIZE - 1);
                            if (username_length > 0) {
                                for (ssize_t i = 0; i < username_length; ++i) {
                                    if (client_usernames[client_id][i] == '\n' || client_usernames[client_id][i] == '\r') {
                                        client_usernames[client_id][i] = '\0';
                                        break;
                                    }
                                }
                            } else {
                                fprintf(stderr, "Error reading the username for client %d\n", client_id);
                                SSL_shutdown(ssl);
                                close(connfd);
                                FD_CLR(connfd, &active_fds);
                                client_fds[client_id] = 0;
                                continue;
                            }

                            fprintf(stdout, "Assigned client ID %d to new connection with username: %s\n", client_id, client_usernames[client_id]);
                            break;
                        }
                    }

                    FD_SET(connfd, &active_fds);
                } else {
                    SSL *ssl;
                    int client_id;
                    for (client_id = 0; client_id < MAX_CLIENTS; ++client_id) {
                        if (client_fds[client_id] == i) {
                            break;
                        }
                    }

                    if ((ssl = SSL_new(ctx)) == NULL) {
                        ERR_print_errors_fp(stderr);
                        close(i);
                        FD_CLR(i, &active_fds);
                        client_fds[client_id] = 0;
                        continue; 
                    }

                    SSL_set_fd(ssl, i);
                    if (SSL_accept(ssl) <= 0) {
                        ERR_print_errors_fp(stderr);
                        close(i);
                        FD_CLR(i, &active_fds);
                        client_fds[client_id] = 0;
                        SSL_free(ssl);
                        continue;  
                    }

                    int nbytes = SSL_read(ssl, buffer, sizeof(buffer));

                    if (nbytes <= 0) {
                        fprintf(stdout, "Connection closed by client %d (%s)\n", client_id, client_usernames[client_id]);
                        SSL_shutdown(ssl);
                        close(i);
                        FD_CLR(i, &active_fds);
                        client_fds[client_id] = 0;
                        SSL_free(ssl);
                    } else {
                        // Broadcast message to all clients with information about the sender
                        for (int j = 0; j < FD_SETSIZE; ++j) {
                            if (FD_ISSET(j, &active_fds) && j != listenfd && j != i) {
                                struct sockaddr_in sender_addr;
                                socklen_t sender_addr_len = sizeof(sender_addr);
                                getpeername(i, (struct sockaddr*)&sender_addr, &sender_addr_len);
                                dprintf(j, "From %s:%d (client %d - %s): %.*s",
                                        inet_ntoa(sender_addr.sin_addr),
                                        ntohs(sender_addr.sin_port),
                                        client_id,
                                        client_usernames[client_id],
                                        nbytes, buffer);
                            }
                        }
                    }

                    SSL_shutdown(ssl);
                    SSL_free(ssl);
                }
            }
        }
    }

    SSL_CTX_free(ctx);
    EVP_cleanup();
    return 0;
}
