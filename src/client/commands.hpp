#ifndef __COMMANDS__HPP__
#define __COMMANDS__HPP__

enum Command{
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
};

/**
 * Handles a login command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_login(char *args, bool *logged_in);

/**
 * Handles a changePass command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_change_pass(char *args, bool *logged_in);

/**
 * Handles a unregister command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_unregister(char *args, bool *logged_in);

/**
 * Handles a logout command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_logout(char *args, bool *logged_in);

/**
 * Handles an exit command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 * @param quit Pointer to a bool that represents if the program exits or not.
 */
void handle_exit(char *args, bool *logged_in, bool *quit);

/**
 * Handles a create command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_create(char *args, bool *logged_in);

/**
 * Handles a close command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_close(char *args, bool *logged_in);

/**
 * Handles a myevents command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_myevents(char *args, bool *logged_in);

/**
 * Handles a list command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_list(char *args, bool *logged_in);

/**
 * Handles a show command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_show(char *args, bool *logged_in);

/**
 * Handles a reserve command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_reserve(char *args, bool *logged_in);

/**
 * Handles a myreservations command.
 * 
 * @param args String with arguments.
 * @param logged_in Pointer to a bool that represent if user is logged in.
 */
void handle_myreservations(char *args, bool *logged_in);

#endif