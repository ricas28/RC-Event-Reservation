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

#endif