#ifndef __S__COMMANDS__HPP__
#define __S__COMMANDS__HPP__

#include "../common/protocol.hpp"

/**
 * Handles a request from a client.
 * 
 * @param fd Socket to write to client.
 * @param code Operation being performed.
 * @param request Request that was sent.
 */
void handle_request(int fd, OP_CODE code, const char *request);

#endif