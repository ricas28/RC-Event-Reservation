#ifndef __PARSER__HPP__
#define __PARSER__HPP__

#include <string>

using namespace std;

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
 * Parses a command, separating the actual command from the arguments.
 * 
 * @param line String with the whole command.
 * @param args Pointer to a string that will contain the arguments.
 * 
 * @returns Command being executed.
 */
enum Command parse_command(char *line, char **args);

/**
 * Parses the command line arguments.
 * 
 * @param port Addres of string representing the server's port.
 * (If this parameter is not present it will be equal to DEFAULT_PORT: 58009)
 * @param ip Addres of a string representing the server's ip.
 * (If this parameter is not present, we assume the server is hosted locally)
 * @param argv Command line arguments.
 * @param argc Number of arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_args(string &port, string &ip, char** argv, int argc);

/* -------------------------------------------------------------------------- */

/**
 * Parses a login command.
 * 
 * @param args String with arguments.
 * @param uid Pointer to store user id.
 * @param pass Pointer to password.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_login(char *args, int *uid, string *pass);

/**
 * Parses a changePass command.
 * 
 * @param args String with arguments.
 * @param old_pass Pointer to old password.
 * @param new_pass Pointer to new password.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_change_pass(char *args, string *old_pass, string *new_pass);

#endif