#pragma once

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