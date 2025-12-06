#ifndef __CLIENT__HPP__
#define __CLIENT__HPP__

#include <iostream>
using namespace std;

struct CLArgs{
    int uid = -1;
    string pass = "";

    int udp_socket = -1;
    struct addrinfo *udp_addr = NULL;

    struct addrinfo *tcp_addr = NULL;

    bool logged_in = false;
};

/**
 * Makes a request to the server sending a message and returning the response,
 * using UDP.
 * 
 * @param client Pointer to client.
 * @param msg Message being sent to the server.
 * 
 * @returns Server response on success, NULL on failure.
 * 
 * Note: Message that is returned needs to be freed using free().
 */
char *client_udp_request(CLArgs *client, const char *msg);

/**
 * Makes a request to the server sending a message and returning the response,
 * using TCP.
 * This function is used for simple requests and responses (without files 
 * involved).
 * 
 * @param client Pointer to client.
 * @param msg Message being sent to the server.
 * 
 * @returns Server response on success, empty string ("") on failure.
 */
string client_tcp_request_line(CLArgs* client, const string &msg);

/** 
 * Opens a tcp connection for the client.
 * 
 * @param client Client.
 * 
 * @returns fd for writing messages to the server.
 * 
 * Note: After using the returned fd, close(fd) needs to be executed.
 */
int client_connect_tcp(CLArgs &client);

/**
 * Initializes the main client's attributes.
 * 
 * @param client Pointer to a client.
 * @param ip IP where the server is running.
 * @param port Port where the server is running.
 * 
 * @returns 0 on success, -1 on failure.
 */
int client_init(CLArgs *client, string ip, string port);

/**
 * Frees memory used by a client.
 * 
 * @param client Pointer to a client.
 */
void destroy_client(CLArgs *client);

#endif