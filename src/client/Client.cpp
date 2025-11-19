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

int set_client_udp_sokcet(CLArgs *client, string ip, string port){
    struct addrinfo hints, *res;

    client->udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->udp_socket == -1) {
        perror("Failure to create UDP socket");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP Socket

    int errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &client->udp_addr);
    if (errcode != 0) {
        exit(1);
    }
}

int set_client_tcp_socket(ClLArgs *client, string ip, string port){
    struct addrinfo hints, *res;
    struct sockaddr_in addr;

    client->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client->tcp_socket == -1) {
        perror("Failure to create TCP socket");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPV 4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    int errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &client->tcp_addr);
    if (errcode != 0) {
        exit(1);
    }
}

void client_init(CLArgs *client, string ip, string port){
    set_client_udp_sokcet(client, ip, port);
    set_client_tcp_socket(client, ip, port);
}