#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "parser.hpp"

using namespace std;

/**
 * Processes a command from the STDIN.
 * 
 * @returns true if successful processing, false otherwise
 * (or if an 'exit' command is executed).
 */
bool process_commands(){
    char line[BUFFER_SIZE], *args;

    cout << "> ";
    if(parse_line(line) == -1) return false;

    switch(parse_command(line, &args)){
        case CMD_LOGIN:
            //if(parse_login(args))
              //  er_login()
            break;
        case CMD_CHANGE_PASS:
            break;
        case CMD_UNREGISTER:
            break;
        case CMD_LOGOUT:
            break;
        case CMD_EXIT:
            // TODO: Implement logout needing to be done first before exit.
            return false;
            break;
        case CMD_CREATE:
            break;
        case CMD_CLOSE:
            break;
        case CMD_MYEVENTS:
            break;
        case CMD_LIST:
            break;
        case CMD_SHOW:
            break;
        case CMD_RESERVE:
            break;
        case CMD_MYRESERVATIONS:
            break;
        case CMD_INVALID:
            cerr << "Invalid/Unknown command" << endl;
            break;
    }   
    return 1;
}

int main(int argc, char** argv){
    string port, ip;

    if(!parse_args(port, ip, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-n ESIP] [-p ESport]" << endl;
        exit(EXIT_FAILURE);
    }
    
    cout << "Insert commands below:" << endl;
    while(1){
        if(!process_commands())
            exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}