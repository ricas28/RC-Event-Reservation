#include <iostream>
#include <string>
#include <cstring>

#include "../common/constants.h"

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
    bool seen_ip = false, seen_port = false;
    // Max number of arguments is 5.
    if(argc > 5)
        return 0;

    // Loop through every argument, ignoring argv[0].
    for(int i = 1; i < argc; i += 2){
        // IP is given.
        if(strcmp(argv[i], "-n") == 0 && !seen_ip && i + 1 < argc){
            ip = argv[i + 1];
            seen_ip = true;
        } 
        // Port is given.
        else if(strcmp(argv[i], "-p") == 0 && !seen_port && i + 1 < argc){
            port = argv[i + 1];
            seen_port = true;
        }
        else{
            return 0;
        }
    }
    // In case one or more arguments weren't present
    if(!seen_ip) ip = DEFAULT_IP;
    if(!seen_port) port = DEFAULT_PORT;
    return 1;
}

int main(int argc, char** argv){
    string port, ip;

    if(!parse_args(port, ip, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-n ESIP] [-p ESport]" << endl;
        return 1;
    }
    cout << "IP: " << ip << " Port: " << port << endl;
    return 0;
}