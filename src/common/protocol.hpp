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
#define EVENT_CORRUPTED -1

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
    ERR,
    INTERNAL_ERROR,
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
struct Event_show_Info{
    int uid;
    string name;
    DateTime event_date;
    int attendace_size;
    int seats_reserved;
    string Fname;
    size_t Fsize;
    string Fdata;
};

// User for 'create' command.
struct Event_creation_Info{
    string name;
    DateTime event_date;
    int attendace_size;
    string Fname;
    ssize_t Fsize;
};

/** Converts the OP_CODE enum value to its string representation. */
string op_to_str(OP_CODE code);
/** Converts a string representation of an operation to its OP_CODE enum value */
OP_CODE str_to_op(const char *str);
/** Checks if the given command has a UID on the request */
bool has_uid(OP_CODE code);

/**
 * Sends a message with UDP.
 * 
 * @param socket Fd for socket.
 * @param message Message being sent.
 * @param addr Pointer do address where we send the message.
 * @param addr_len Size of address.
 * 
 * @returns 0 on success, -1 on failure.
 */
int send_udp_message(int socket, const char *message, struct sockaddr *addr, socklen_t addr_len);

/**
 * Receives a message with UDP.
 * 
 * @param socket Fd for socket.
 * @param addr Pointer to address where we receive the message from.
 * @param socklen_t Pointer to size of address.
 * 
 * @returns pointer to buffer with content on success, NULL on failure.
 */
char *receive_udp_message(int socket,  struct sockaddr *addr, socklen_t *addr_len);

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
 * Streams the content of a file with TCP
 * 
 * @param sendingfd Fd of the sender.
 * @param receivingfd Fd that is receiving content.
 * @param Fsize Size of the file.
 * 
 * @returns Bytes that were sent.
 */
ssize_t stream_file_TCP(int sendingfd, int receivingfd, size_t Fsize);

#endif