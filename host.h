#ifndef HOST_H
#define HOST_H

typedef void (*message_handler_t)(int sock);

void start_server(message_handler_t server_handler, const int max_connections);
void start_client(message_handler_t client_handler, const char *server_ip);

#endif
