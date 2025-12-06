#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <signal.h>

#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "../common/protocol.hpp"
#include "parser.hpp"
#include "commands.hpp"
#include "Client.hpp"

using namespace std;

/**
 * Processes commands from the STDIN.
 * 
 * @param client Pointer to client arguments.
 */
void process_commands(CLArgs *client){
    bool quit = false;
    char *args, line[BUFFER_SIZE];

    cout << "Insert commands below:" << endl;
    while(!quit){
        // Flush because read_line uses read().
        cout << "> " << flush;
        if(read_line_256(STDIN_FILENO, line) == -1) exit(EXIT_FAILURE);

        // TODO: (Optional) Add Help command
        switch(parse_command(line, &args)){
            case CMD_LOGIN:
                handle_login(args, client);
                break;
            case CMD_CHANGE_PASS:
                handle_changePass(args, client);
                break;
            case CMD_UNREGISTER:
                handle_unregister(args, client);
                break;
            case CMD_LOGOUT:
                handle_logout(args, client);
                break;
            case CMD_EXIT:
                handle_exit(args, client, &quit);
                break;
            case CMD_CREATE:
                handle_create(args, client);
                break;
            case CMD_CLOSE:
                handle_close(args, client);
                break;
            case CMD_MYEVENTS:
                handle_myevents(args, client);
                break;
            case CMD_LIST:
                handle_list(args, client);
                break;
            case CMD_SHOW:
                handle_show(args, client);
                break;
            case CMD_RESERVE:
                handle_reserve(args, client);
                break;
            case CMD_MYRESERVATIONS:
                handle_myreservations(args, client);
                break;
            case CMD_INVALID:
                cerr << "Invalid/Unknown command!\nSee help for usage." << endl;
                break;
        }   
    }
}

int main(int argc, char** argv){
    string port, ip;
    CLArgs client;

    signal(SIGPIPE, SIG_IGN); // Ignore broken pipe.
    if(!parse_args(port, ip, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-n ESIP] [-p ESport]" << endl;
        exit(EXIT_FAILURE);
    }    

    if(client_init(&client, ip, port) == -1){
        cerr << "Failure to initialize client parameters" << endl;
        exit(EXIT_FAILURE);
    }
    process_commands(&client);
    destroy_client(&client);
    return EXIT_SUCCESS;
}