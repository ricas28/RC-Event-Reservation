#ifndef __S__COMMANDS__HPP__
#define __S__COMMANDS__HPP__

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../common/protocol.hpp"

struct UDPSender{
    int fd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;
};

/**
 * Processes a UDP request from a client.
 * 
 * @param sender UDPSender struct.
 * @param code Code that was sent.
 * @param request Request that was sent.
 */
void process_UDP_request(UDPSender sender, OP_CODE code, const char *request);

/**
 * Processes a TCP request from a client.
 * 
 * @param fd TCP socket.
 * @param code Code that was sent.
 * @param request Request that was sent.
 */
void process_TCP_request(int fd, OP_CODE code, const char *request);

#endif