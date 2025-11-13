#include <iostream>
#include <unistd.h>
#include <string.h>

#include "parser.h"

using namespace std;

#define COMMAND_SIZE 16

/**
 * Parses the received command.
 * 
 * @param buf String with command that will be performed.
 * 
 * @returns Command being performed.
 */
enum Command get_command(char *buf){
    if(strcmp(buf, "login") == 0) return CMD_LOGIN;
    else if(strcmp(buf, "changePass") == 0) return CMD_CHANGE_PASS;
    else if(strcmp(buf, "unregister") == 0) return CMD_UNREGISTER;
    else if(strcmp(buf, "logout") == 0) return CMD_LOGOUT;
    else if(strcmp(buf, "exit") == 0) return CMD_EXIT;
    else if(strcmp(buf, "create") == 0) return CMD_CREATE;
    else if(strcmp(buf, "close") == 0) return CMD_CLOSE;
    else if(strcmp(buf, "mye") == 0 || strcmp(buf, "myevents") == 0) return CMD_MYEVENTS;
    else if(strcmp(buf, "list") == 0) return CMD_LIST;
    else if(strcmp(buf, "show") == 0) return CMD_SHOW;
    else if(strcmp(buf, "reserve") == 0) return CMD_RESERVE;
    else if(strcmp(buf, "myr") == 0 || strcmp(buf, "myreservations") == 0) return CMD_MYRESERVATIONS;
    else return CMD_INVALID;
}

enum Command parse_command(char *line, char **args){
    char command[COMMAND_SIZE];
    if(sscanf(line, "%s", command) < 1){
        perror("Failure to parse command");
    }

    size_t command_size = strlen(command);
    // Put args pointing to the next space after the command.
      if (line[command_size] == ' ') {
        *args = line + command_size + 1;

        // Remove newline from the end, if it exists.
        size_t len = strlen(*args);
        if (len > 0 && (*args)[len-1] == '\n')
            (*args)[len-1] = '\0';
    } else {
        *args = NULL;
    }

    return get_command(command);
}