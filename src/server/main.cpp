#include <iostream>
#include <signal.h>

#include "parser.hpp"
#include "server.hpp"

using namespace std;

void process_commands(SVArgs server){

}

int main(int argc, char **argv){
    string port;
    bool verbose;
    SVArgs server;

    signal(SIGPIPE, SIG_IGN); // Ignore broken pipe.
    if(!parse_args(port, verbose, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-p ESport] [-v]" << endl;
        exit(EXIT_FAILURE);
    }
    if(server_init(server, port, verbose) == -1){
        cerr << "Failure to initialize server sockets";
        exit(EXIT_FAILURE);
    }
    process_commands(server);
    destroy_server(server);
    exit(EXIT_SUCCESS);
}