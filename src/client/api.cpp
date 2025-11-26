#include <iostream>

#include "Client.hpp"
#include "parser.hpp"
#include "../common/protocol.hpp"

using namespace std;

int er_login(CLArgs client, int uid, string pass){
    char *response;
    string message = string(op_to_str(OP_LOGIN)) + " " + 
                        to_string(uid) + " " + pass + "\n";

    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }
    string status = "";
    if(!parse_login_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        return -1;
    }
    // Print message according to status value
    if(status == "OK") cout << "successful login" << endl;
    else if(status == "NOK") cout << "incorrect login attempt" << endl;
    else if(status == "REG") cout << "new user registered" << endl;
    else cerr << "[API] Invalid status message passed through" << endl;
    free(response);
    return 0;
}