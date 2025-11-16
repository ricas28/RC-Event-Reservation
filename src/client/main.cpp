#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "parser.hpp"

using namespace std;

/**
 * Checks if a user is logged in. Printing a message if not.
 * 
 * @param logged_in Bool indicating if user is logged_in.
 * 
 * @returns true if user is logged in, false otherwise.
 */
bool is_logged_in(bool logged_in){
    if(!logged_in){
        cout << "user not logged In" << endl;
        return false;
    }
    return true;
}

/**
 * Processes commands from the STDIN.
 */
void process_commands(){
    bool logged_in = false, quit = false;
    char line[BUFFER_SIZE], *args;
    // Client information
    int uid; 
    string pass;

    while(!quit){
        cout << "> ";
        if(parse_line(line) == -1) exit(EXIT_FAILURE);

        // TODO: (Optional) Add Help command
        switch(parse_command(line, &args)){
            case CMD_LOGIN: {
                if(parse_login(args, &uid, &pass)){
                    //er_login(&logged_in);
                }
                break;
            }
            case CMD_CHANGE_PASS: {
                if(!is_logged_in(logged_in)) break;
                string old_pass, new_pass;
                if(parse_change_pass(args, &old_pass, &new_pass)){
                    //er_change_pass();
                }
                break;
            }
            case CMD_UNREGISTER:
                if(!is_logged_in(logged_in)) break;
                if(parse_unregister(args)){
                    //er_unregister();
                }
                break;
            case CMD_LOGOUT:
                if(!is_logged_in(logged_in)) break;
                if(parse_logout(args)){
                    //er_logout(&logged_in);
                }
                break;
            case CMD_EXIT:
                if(parse_exit(args)){
                    if(logged_in){
                        cout << "Currently logged in!" << endl;
                        cout << "Please execute the 'logout' command first" << endl;
                    }
                    else quit = true;
                }
                break;
            case CMD_CREATE: {
               // string name, event_fname, num_attendes;
               // if(parse_create(args)){
                //    er_create()
                //}
                break;
            }
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