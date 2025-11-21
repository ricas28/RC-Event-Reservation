#ifndef __PROTOCOL__HPP__
#define __PROTOCOL__HPP__

#include <sys/types.h>
#include <sys/socket.h>

typedef enum {
    OP_LOGIN,
    OP_LOGIN_RESP,
    OP_CHANGE_PASS,
    OP_CHANGE_PASS_RESP,
    OP_UNREGISTER,
    OP_UNREGISTER_RESP,
    OP_LOGOUT,
    OP_LOGOUT_RESP,
    OP_CREATE,
    OP_CREATE_RESP,
    OP_CLOSE,
    OP_CLOSE_RESP,
    OP_MYEVENTS,
    OP_MYEVENTS_RESP,
    OP_LIST,
    OP_LIST_RESP,
    OP_RESERVE,
    OP_RESERVE_RESP,
    OP_MYRESERVATIONS,
    OP_MYRESERVATIONS_RESP,
    OP_INVALID
} OP_CODE;

/**
 * Receives an OP_CODE and returnS it's string form.
 * 
 * @param 
 * 
 */
const char* op_to_str(OP_CODE code);
OP_CODE str_to_op(const char *str);

/**
 * Sends a message with UDP.
 * 
 * @param socket Fd for socket.
 * @param message Message being sent.
 * @param addr Pointer do address where we send the message.
 * 
 * @returns 0 on success, -1 on failure.
 */
int send_udp_message(int socket, const char *message, const struct addrinfo *addr);

/**
 * Receives a message with UDP.
 * 
 * @param socket Fd for socket.
 * @param addr Pointer to adress where we receive the message from.
 * 
 * @returns pointer to buffer with content on success, NULL on failure.
 */
char *receive_udp_message(int socket, struct addrinfo *addr);

/**
 * Closes a socket connection by freeing memory allocated to adressing
 * and closing the socket.
 * 
 * @param socket Fd for socket.
 * @param addr Pointer to an adress.
 */
void close_socket_connection(int socket, struct addrinfo *addr);

#endif