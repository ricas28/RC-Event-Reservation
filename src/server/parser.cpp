#include <iostream>
#include <string>
#include <unistd.h>

#include "../common/constants.hpp"

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