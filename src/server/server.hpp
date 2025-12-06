#ifndef __SERVER__HPP__
#define __SERVER__HPP__

#include <iostream>
 
using namespace std;

struct SVArgs{
    int tcp_socket = -1;
    int udp_socket = -1;
    bool verbose = false;
};

/**
 * Initializes the parameters for a SVArgs struct.
 * 
 * @param server Struct being initialized.
 * @param port Port where the server is running.
 * @param verbose Bool to indicate server's verbose mode.
 * 
 * @return 0 on success, -1 on failure.
 */
int server_init(SVArgs &server, string port, bool verbose);

/** Closes file descriptors for TCP and UDP messages.*/
void destroy_server(SVArgs &server);

#endif