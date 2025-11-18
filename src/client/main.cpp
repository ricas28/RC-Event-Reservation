#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "parser.hpp"
#include "commands.hpp"
#include "Client.hpp"

using namespace std;

/**
 * Processes commands from the STDIN.
 */
void process_commands(){
    bool quit = false;
    char line[BUFFER_SIZE], *args;
    // Client information
    CLArgs client;

    // TODO: Implement client class that has uid, pass and logged_in status.
    while(!quit){
        // Flush because read_line uses read().
        cout << "> " << flush;
        if(read_line(STDIN_FILENO, line) == -1) exit(EXIT_FAILURE);

        // TODO: (Optional) Add Help command
        switch(parse_command(line, &args)){
            case CMD_LOGIN:
                handle_login(args, &client);
                break;
            case CMD_CHANGE_PASS:
                handle_change_pass(args, &client);
                break;
            case CMD_UNREGISTER:
                handle_unregister(args, &client);
                break;
            case CMD_LOGOUT:
                handle_logout(args, &client);
                break;
            case CMD_EXIT:
                handle_exit(args, &client, &quit);
                break;
            case CMD_CREATE:
                handle_create(args, &client);
                break;
            case CMD_CLOSE:
                handle_close(args, &client);
                break;
            case CMD_MYEVENTS:
                handle_myevents(args, &client);
                break;
            case CMD_LIST:
                handle_list(args, &client);
                break;
            case CMD_SHOW:
                handle_show(args, &client);
                break;
            case CMD_RESERVE:
                handle_reserve(args, &client);
                break;
            case CMD_MYRESERVATIONS:
                handle_myreservations(args, &client);
                break;
            case CMD_INVALID:
                cerr << "Invalid/Unknown command!\nSee help for usage." << endl;
                break;
        }   
    }
}

int main(int argc, char** argv){
    string port, ip;

    if(!parse_args(port, ip, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-n ESIP] [-p ESport]" << endl;
        exit(EXIT_FAILURE);
    }
    
    cout << "Insert commands below:" << endl;
    process_commands();
    return EXIT_SUCCESS;
}