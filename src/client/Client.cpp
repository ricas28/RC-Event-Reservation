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
#include "../common/constants.hpp"
#include "../common/protocol.hpp"
#include "../common/util.hpp"

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

    struct timeval tv;
    tv.tv_sec = UDP_TIMEOUT;
    tv.tv_usec = 0;
    if (setsockopt(client->udp_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        close(client->udp_socket);
        perror("Failed to set socket receive timeout");
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP Socket

    int errcode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &client->udp_addr);
    if (errcode != 0) {
        close(client->udp_socket);
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

char *client_udp_request(CLArgs *client, const char *msg){
    // Optional structs for receiving the server's ip.
    struct sockaddr_storage reply_addr;
    socklen_t reply_len = sizeof(reply_addr);

    for (int attempt = 1; attempt <= UDP_RETRIES; attempt++) {
        if (send_udp_message(client->udp_socket, msg, 
                                        client->udp_addr->ai_addr,
                                        client->udp_addr->ai_addrlen) == -1)
            return NULL;
        char *resp = receive_udp_message(client->udp_socket, (struct sockaddr*)&reply_addr,
                                                                            &reply_len);
        if (resp != NULL)
            return resp;
        
        // Timeout error.
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            free(resp);
            cerr << "[UDP] Timeout (" << attempt << "/" << UDP_RETRIES << ")";
            if(attempt <= UDP_RETRIES - 1)
                cerr << ", retrying..." << endl;
            else 
                cerr << ". Max retries reached..." << endl;
            continue;
        }
    }
    return NULL;  // no response after UDP_RETRIES attempts
}

int client_connect_tcp(CLArgs &client){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Failure to open TCP socket");
        return -1;
    }

    // Connect to server.
    int n = connect(fd, client.tcp_addr->ai_addr, client.tcp_addr->ai_addrlen);
    if (n == -1) {
        perror("Failure to connect to server with TCP");
        close(fd);
        return -1;
    }
    return fd;
}

string client_tcp_request_line(CLArgs* client, const string &msg){
    int fd = client_connect_tcp(*client);

    if(fd == -1)
        return "";

    // Write to server.
    if(write_all(fd, msg.c_str(), msg.size()) == -1){
        perror("Failure to write to server with TCP");
        close(fd);
        return "";
    }

    // Receive response.
    string response;
    if((response = tcp_read_message(fd)) == ""){
        perror("Failure to read from server with TCP");
        close(fd);
        return "";
    }

    // Close session
    close(fd);
    return response;
}

int client_init(CLArgs *client, string ip, string port){
    if(set_client_udp_socket(client, ip, port) == -1)
        return -1;
    if(set_client_tcp(client, ip, port) == -1){
        freeaddrinfo(client->udp_addr);
        close(client->udp_socket);
        return -1;
    }
    return 0;
}

void destroy_client(CLArgs *client){
    freeaddrinfo(client->udp_addr);
    freeaddrinfo(client->tcp_addr);
    close(client->udp_socket);
}