#include <iostream>

#include "Client.hpp"
#include "../common/protocol.hpp"

using namespace std;

int er_login(CLArgs client, int uid, string pass){
    char *response;
    string message = string(op_to_str(OP_LOGIN)) + " " + 
                        to_string(uid) + " " + pass + "\n";

    if(send_udp_message(client.udp_socket, message.c_str(), client.udp_addr) == -1)
        return -1;
    if((response = receive_udp_message(client.udp_socket, client.udp_addr)) == NULL)
        return -1;
    
    free(response);
    return 0;
}