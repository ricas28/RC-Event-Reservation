#include <iostream>

#include "parser.hpp"

using namespace std;

int main(int argc, char **argv){
    string port;
    bool verbose;

    if(!parse_args(port, verbose, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-p ESport] [-v]" << endl;
        exit(EXIT_FAILURE);
    }
    cout << port << endl << verbose << endl;
    exit(EXIT_SUCCESS);
}