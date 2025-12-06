#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "server.hpp"

using namespace std;

#define MAX_CONNECTIONS 5

/**
 * Sets up the TCP server socket.
 *  
 * @param port Port where the server is running.
 * 
 * @returns Fd for socket on success, -1 on failure.
 */
int set_server_tcp_socket(string port){
    struct addrinfo hints, *res;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror ("Failure to open TCP socket");
        return -1;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if ((errcode) != 0) {
        perror("Failure to do DNS lookup for TCP socket");
        close(fd);
        return -1;
    }
    int n = bind(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        perror("Failure to bind TCP socket to adress");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }
    if (listen(fd, MAX_CONNECTIONS) == -1) {
        perror("Failure to execute listen function");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }

    freeaddrinfo(res);
    return fd;
}

/**
 * Sets up the UDP server socket.
 *  
 * @param port Port where the server is running.
 * 
 * @returns Fd for socket on success, -1 on failure.
 */
int set_server_udp_socket(string port){
    struct addrinfo hints, *res;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if (errcode != 0) {
        close(fd);
        return -1;
    }
    int n = bind(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        freeaddrinfo(res);
        close(fd);
        return -1;
    }

    freeaddrinfo(res);
    return fd;
}

int server_init(SVArgs &server, string port, bool verbose){
    if((server.tcp_socket = set_server_tcp_socket(port)) == -1)
        return -1;
    
    if((server.udp_socket = set_server_udp_socket(port)) == -1){
        close(server.tcp_socket);
        return -1;
    }
    server.verbose = verbose;
    return 0;
}

void destroy_server(SVArgs &server){
    close(server.tcp_socket);
    close(server.udp_socket);
}