#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

#include "../common/constants.hpp"
#include "../common/util.hpp"
#include "parser.hpp"

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

bool parse_args(string &port, string &ip, char** argv, int argc){
    int  opt;

    // Max number of arguments is 5.
    if(argc > 5)
        return false;

    // Put default argument values in case they are not present.
    ip = DEFAULT_IP;
    port = DEFAULT_PORT;

    // opt will be option on argv and optarg is the next item on argv.
    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ip = optarg;   
                break;
            case 'p':
                port = optarg;
                break;
            // Unknown command (neither -n nor -p)
            case '?':
                return false;
        }
    }
    return true;
}

enum Command parse_command(char *line, char **args){
    char command[COMMAND_SIZE];
    if(sscanf(line, "%15s", command) < 1){
        cout << "Failure to parse command!" << endl;
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

/* ------------------------------------------------------------------------- */
bool parse_login(char *args, int *uid, string *pass){
    bool error = false;
    char uid_temp[BUF_TEMP], pass_temp[BUF_TEMP], extra[BUF_TEMP];

    int n = sscanf(args, "%63s %63s %255s", uid_temp, pass_temp, extra);
    if (n < 2) {
        cout << "Invalid arguments! Not enough arguments." << endl;
        cout << "Usage: login <userID> <password>" << endl;
        return false;
    }
    if (n > 2) {
        cout << "Invalid arguments! Too many arguments." << endl;
        cout << "Usage: login <userID> <password>" << endl;
        return false;
    }
    if (!is_valid_userid(uid_temp)) {
        cout << "Invalid userID! Must be exactly 6 digits." << endl;
        error = true;
    }
    if (!is_valid_password(pass_temp)) {
        cout << "Invalid password! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if (error) return false;
    // Successful parse.
    *uid = atoi(uid_temp);
    *pass = pass_temp;

    return true;
}

bool parse_change_pass(char *args, string *old_pass, string *new_pass){
    bool error = false;
    char old_pass_temp[BUF_TEMP], new_pass_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(args, "%63s %63s %255s", old_pass_temp, new_pass_temp, extra);
    if (n < 2) {
        cout << "Invalid arguments! Not enough arguments." << endl;
        cout << "Usage: changePass <oldPassword> <newPassword>" << endl;
        return false;
    }
    if (n > 2) {
        cout << "Invalid arguments! Too many arguments." << endl;
        cout << "Usage: changePass <oldPassword> <newPassword>" << endl;
        return false;
    }
    if (!is_valid_password(old_pass_temp)) {
        cout << "Old password is invalid! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if (!is_valid_password(new_pass_temp)) {
        cout << "New password is invalid! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if(strcmp(old_pass_temp, new_pass_temp) == 0){
        cout << "Passwords can't be equal!" << endl;
        error = true;
    }
    if (error) return false;
    // Successful parse.
    *old_pass = old_pass_temp;
    *new_pass = new_pass_temp;
    return true; 
}