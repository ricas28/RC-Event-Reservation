#include <iostream>
#include <string>
#include <unistd.h>

#include "../common/constants.hpp"
#include "../common/protocol.hpp"

using namespace std;

bool parse_args(string &port, bool &verbose, char** argv, int argc){
    int  opt;

    // Max number of arguments is 4.
    if(argc > 4)
        return false;

    // Put default argument values in case they are not present.
    port = DEFAULT_PORT;
    verbose = false;

    // opt will be option on argv and optarg is the next item on argv.
    while ((opt = getopt(argc, argv, "p:v")) != -1) {
        switch (opt) {
            case 'p':
                port = optarg;   
                break;
            case 'v':
                verbose = true;
                break;
            // Unknown command (neither -p nor -v)
            case '?':
                return false;
        }
    }

    // Arguments beyond the options.
    if (optind < argc) {
        return false;
    }
    return true;
}

OP_CODE get_tcp_command(const char *command){
    OP_CODE code = str_to_op(command);
    switch(code){
        case OP_CREATE:
        case OP_CLOSE:
        case OP_LIST:
        case OP_SHOW:
        case OP_RESERVE:
        case OP_CHANGE_PASS:
            return code;
            break;
        default:
            cerr << "Unexpected command received through TCP: " << command << endl;
            return ERR;
    }
}

OP_CODE get_udp_command(const char *command){
    OP_CODE code = str_to_op(command);
    switch(code){
        case OP_LOGIN:
        case OP_LOGOUT:
        case OP_UNREGISTER:
        case OP_MYEVENTS:
        case OP_MYRESERVATIONS:
            return code;
            break;
        default:
            cerr << "Unexpected command received through UDP: " << command << endl;
            return ERR;
    }
}