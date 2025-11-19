#include <sys/types.h>
#include <sys/socket.h>

int send_udp_message(int socket, const char *message, const struct addrinfo *addr);

int receive_udp_message(int socket, struct addrinfo *addr);