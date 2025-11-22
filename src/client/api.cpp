#include <iostream>

#include "Client.hpp"
#include "../common/protocol.hpp"

using namespace std;

int er_login(CLArgs client, int uid, string pass){
    char *response;
    string message = string(op_to_str(OP_LOGIN)) + " " + 
                        to_string(uid) + " " + pass + "\n";

    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to execute login command" << endl;
    }
    //process_login_response(response);
    free(response);
    return 0;
}