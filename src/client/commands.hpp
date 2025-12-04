#ifndef __COMMANDS__HPP__
#define __COMMANDS__HPP__

#include "Client.hpp"

typedef enum{
    CMD_LOGIN,
    CMD_CREATE,
    CMD_CLOSE,
    CMD_MYEVENTS,
    CMD_LIST,
    CMD_SHOW,
    CMD_RESERVE,
    CMD_MYRESERVATIONS,
    CMD_CHANGE_PASS,
    CMD_UNREGISTER,
    CMD_LOGOUT,
    CMD_EXIT,
    CMD_INVALID
}Command;

// Used for parsing of 'myreservations' response.
struct Reservation {
    string eid;
    DateTime datetime;
    int value;
};

// Used for parsing 'list' response.
struct Event{
    string eid;
    string name;
    int state;
    DateTime datetime;
};

/**
 * Handles a 'login' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_login(char *args, CLArgs *client);

/**
 * Handles a 'changePass' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_changePass(char *args, CLArgs *client);

/**
 * Handles a 'unregister' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_unregister(char *args, CLArgs *client);

/**
 * Handles a 'logout' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_logout(char *args, CLArgs *client);

/**
 * Handles an 'exit' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 * @param quit Pointer to a bool that represents if the program exits or not.
 */
void handle_exit(char *args, CLArgs *client, bool *quit);

/**
 * Handles a 'create' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_create(char *args, CLArgs *client);

/**
 * Handles a 'close' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_close(char *args, CLArgs *client);

/**
 * Handles a 'myevents' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_myevents(char *args, CLArgs *client);

/**
 * Handles a 'list' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_list(char *args, CLArgs *client);

/**
 * Handles a 'show' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_show(char *args, CLArgs *client);

/**
 * Handles a 'reserve' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_reserve(char *args, CLArgs *client);

/**
 * Handles a 'myreservations' command.
 * 
 * @param args String with arguments.
 * @param client Pointer to client attributes.
 */
void handle_myreservations(char *args, CLArgs *client);

#endif