#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "Client.hpp"
#include "../common/io.hpp"

/**
 * Initializes a UDP socket and address.
 * 
 * @param client Pointer to a client.
 * @param ip Ip where the server is being hosted.
 * @param port Port where the server is being hosted.
 * 
 * @returns 0 on success, -1 on failure.
 */
int set_client_udp_socket(CLArgs *client, string ip, string port){
    struct addrinfo hints;

    client->udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->udp_socket == -1) {
        perror("Failure to create UDP socket");
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP Socket

    int errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &client->udp_addr);
    if (errcode != 0) {
        perror("Failure executing getaddrinfo for UDP socket");
        return -1;
    }
    return 0;
}

/**
 * Initializes a TCP address.
 * 
 * @param client Pointer to a client.
 * @param ip Ip where the server is being hosted.
 * @param port Port where the server is being hosted.
 * 
 * @returns 0 on success, -1 on failure.
 */
int set_client_tcp(CLArgs *client, string ip, string port){
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    int errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &client->tcp_addr);
    if (errcode != 0) {
        return -1;
    }

    return 0;
}

int client_init(CLArgs *client, string ip, string port){
    if(set_client_udp_socket(client, ip, port) == -1)
        return -1;
    if(set_client_tcp(client, ip, port) == -1)
        return -1;
    return 0;
}

void destroy_client(CLArgs *client){
    freeaddrinfo(client->udp_addr);
    freeaddrinfo(client->tcp_addr);
    close(client->udp_socket);
}