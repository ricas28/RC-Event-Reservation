#include <iostream>

#include "parser.hpp"
#include "server.hpp"

using namespace std;

int main(int argc, char **argv){
    string port;
    bool verbose;
    SVArgs server;

    if(!parse_args(port, verbose, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-p ESport] [-v]" << endl;
        exit(EXIT_FAILURE);
    }
    if(server_init(server, port, verbose) == -1){
        cerr << "Failure to initialize server sockets";
        exit(EXIT_FAILURE);
    }
    destroy_server(server);
    exit(EXIT_SUCCESS);
}