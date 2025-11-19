#ifndef __CLIENT__HPP__
#define __CLIENT__HPP__

#include <iostream>
using namespace std;

typedef struct ClLArgs{
    int uid = -1;
    string pass = "";

    int udp_socket = -1;
    int tcp_socket = -1;

    struct addrinfo *tcp_addr = NULL;
    struct addrinfo *udp_addr = NULL;

    bool logged_in = false;
}CLArgs;

/**
 * Initializes the main client's attributes.
 * 
 * @param client Pointer to a client.
 * @param ip IP where the server is running.
 * @param port Port where the server is running.
 */
void client_init(CLArgs *client, string ip, string port);

#endif