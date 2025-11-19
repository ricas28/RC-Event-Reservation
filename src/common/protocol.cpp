#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "constants.hpp"

using namespace std;

int send_udp_message(int socket, const char *message, const struct addrinfo *addr){
    ssize_t n = sendto(socket, message, strlen(message), 0, addr->ai_addr, addr->ai_addrlen);
    if (n == -1) {
        perror("Failure to send message with UDP");
        return -1;
    }
    return 0;
}

int receive_udp_message(int socket, struct addrinfo *addr){
    char buffer[BUFFER_SIZE];

    socklen_t addrlen = sizeof(*addr);
    ssize_t n = recvfrom(socket, buffer, 128, 0, (struct sockaddr *)addr, &addrlen);
    if (n == -1) {
        perror("Failure to receive message with UDP");
        return -1;    
    }
    buffer[n] = '\0'; 
    return 0;
}
