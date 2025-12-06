#ifndef __PROTOCOL__HPP__
#define __PROTOCOL__HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "DateTime.hpp"

using namespace std;

#define EVENT_IN_PAST 0
#define EVENT_ACCEPTING 1
#define EVENT_SOLD_OUT 2
#define EVENT_CLOSED 3

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
    OP_SHOW,
    OP_SHOW_RESP,
    OP_RESERVE,
    OP_RESERVE_RESP,
    OP_MYRESERVATIONS,
    OP_MYRESERVATIONS_RESP,
    OP_INVALID,
    ERR
} OP_CODE;

// Used for 'list' command.
struct Event_list{
    string eid;
    string name;
    int state;
    DateTime datetime;
};

// Used for 'myreservations' command.
struct Reservation {
    string eid;
    DateTime datetime;
    int value;
};

// Used for 'show' command.
struct Event_Info{
    int uid;
    string name;
    DateTime event_date;
    int attendace_size;
    int seats_reserved;
    string Fname;
    size_t Fsize;
    string Fdata;
};

/** Converts the OP_CODE enum value to its string representation. */
string op_to_str(OP_CODE code);
/** Converts a string representation of an operation to its OP_CODE enum value */
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
 * Reads the next message from a TCP socket. Read until seeing '\n'.
 * 
 * @param fd File descriptor.
 * 
 * @returns Next message.
 */
string tcp_read_message(int fd);

/**
 * Reads a word from a TCP socket.
 * 
 * @param fd TCP socket.
 * @param end_line (Optional) Pointer to a bool that tells if
 *  after the word that was read, the line ends or not.
 * 
 * @returns word read from TCP or "" in case of failure.
 */
string tcp_read_word(int fd, bool *end_line = nullptr);

/**
 * Closes a socket connection by freeing memory allocated to adressing
 * and closing the socket.
 * 
 * @param socket Fd for socket.
 * @param addr Pointer to an adress.
 */
void close_socket_connection(int socket, struct addrinfo *addr);

#endif