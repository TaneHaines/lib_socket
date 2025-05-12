#ifndef HOST_H
#define HOST_H

typedef void (*message_handler_t)(int sock);

void start_server(message_handler_t server_handler);
void start_client(const char *server_ip, message_handler_t client_handler);

#endif
