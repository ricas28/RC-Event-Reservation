#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "../common/constants.hpp"
#include "parser.hpp"

#define BUFFER_SIZE 256

using namespace std;

/**
 * Parses the command line arguments and returns 1 when the parse is 
 * successful and 0 otherwise.
 * 
 * @param port Addres of string representing the server's port.
 * (If this parameter is not present it will be equal to DEFAULT_PORT: 58009)
 * @param ip Addres of a string representing the server's ip.
 * (If this parameter is not present, we assume the server is hosted locally)
 * @param argv Command line arguments.
 * @param argc Number of arguments.
 * 
 * @returns 1 if parse is successful, 0 otherwise.
 */
int parse_args(string &port, string &ip, char** argv, int argc){
    int  opt;

    // Max number of arguments is 5.
    if(argc > 5)
        return 0;

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
                return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv){
    string port, ip;
    bool exit = false;

    if(!parse_args(port, ip, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-n ESIP] [-p ESport]" << endl;
        return 1;
    }
    
    char line[BUFFER_SIZE];
    while(!exit){
        if(!fgets(line, sizeof(line), stdin)){
            perror("Failure to read command.");
            break;
        }
        
        char *args;
        // TODO: Create function on the api file and put them here.
        switch(parse_command(line, &args)){
            case CMD_LOGIN:
                break;
            case CMD_CHANGE_PASS:
                break;
            case CMD_UNREGISTER:
                break;
            case CMD_LOGOUT:
                break;
            case CMD_EXIT:
                // TODO: Implement logout needing to be done first before exit.
                exit = true;
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
    }
    return 0;
}