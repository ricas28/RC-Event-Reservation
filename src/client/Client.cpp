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
        perror("Failed to set socket receive timeout");
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

char *client_udp_request(CLArgs *client, const char *msg){
    for (int attempt = 1; attempt <= UDP_RETRIES; attempt++) {
        if (send_udp_message(client->udp_socket, msg, client->udp_addr) == -1)
            return NULL;
        char *resp = receive_udp_message(client->udp_socket, client->udp_addr);
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

string client_tcp_request_line(CLArgs* client, const string &msg){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Failure to open TCP socket");
        return "";
    }

    // Connect to server.
    int n = connect(fd, client->tcp_addr->ai_addr, client->tcp_addr->ai_addrlen);
    if (n == -1) {
        perror("Failure to connect to server with TCP");
        close(fd);
        return "";
    }

    // Write to server.
    if(write_all(fd, msg.c_str(), msg.size()) == -1){
        perror("Failure to write to server with TCP");
        close(fd);
        return "";
    }

    // Receive response.
    string response;
    if(read_until_line_end(fd, response) == -1){
        perror("Failure to read from server with TCP");
        close(fd);
        return "";
    }

    // Close session
    close(fd);
    return response;
}

////////////////////////////////////////////////////////////////////
// TODO: create client_tcp_request_file(). for commands like SHOW //
////////////////////////////////////////////////////////////////////

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