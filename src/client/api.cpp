#include <iostream>

#include "Client.hpp"
#include "parser.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "../common/io.hpp"

using namespace std;

/* -------------- UDP COMMANDS ------------------- */
int er_login(CLArgs client, int uid, string pass){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_LOGIN) + " " + 
                        to_string(uid) + " " + pass + "\n";

    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response to 'login' command" << endl;
    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(response);
        return -1;
    }
    if(code != OP_LOGIN_RESP || !parse_login_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "successful login" << endl;
    else if(status == "NOK") cout << "incorrect login attempt" << endl;
    else if(status == "REG") cout << "new user registered" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else cerr << "[API] Invalid status message passed through" << endl;
    free(response);
    return 0;         
}

int er_logout(CLArgs client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_LOGOUT) + " " + to_string(client.uid) + 
                                            " " + client.pass + "\n";
                                            
    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'logout' command" << endl;
    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(response);
        return -1;
    }
    if(code != OP_LOGOUT_RESP || !parse_logout_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "successful logout" << endl;
    else if(status == "NOK") cout << "user not logged in" << endl;
    else if(status == "UNR") cout << "unknown user" << endl;
    else if(status == "WRP") cerr << "[API] Wrong password sent on 'logout' command" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else cerr << "[API] Invalid status message passed through" << endl;
    free(response);
    return 0;                         
}

int er_unregister(CLArgs client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_UNREGISTER) + " " + to_string(client.uid) + 
                                            " " + client.pass + "\n";
                                            
    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'unregister' command" << endl;
    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(response);
        return -1;
    }
    if(code != OP_UNREGISTER_RESP || !parse_unregister_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "successful unregister" << endl;
    else if(status == "NOK" || status == "WRP") cout << "incorrect unregister attempt" << endl;
    else if(status == "UNR") cout << "unknown user" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else cerr << "[API] Invalid status message passed through" << endl;
    free(response);
    return 0;       
}

/* ----------------------------------------------- */

/* -------------- TCP COMMANDS ------------------- */
int er_create(CLArgs client, string name, string event_fname, size_t Fsize,
                        char *Fdata, DateTime event_date, int num_attendees){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_CREATE) + " " + to_string(client.uid) + " " + client.pass +
                        " " + name + " " + event_date.toString() + " " + to_string(num_attendees) +
                        " " + event_fname + " " + to_string(Fsize) + " " + Fdata + "\n";

    if((response = client_tcp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(Fdata);
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'create' command" << endl;

    string status = "";
    int eid = -1;
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(Fdata);
        free(response);
        return -1;
    }
    if(code != OP_CREATE_RESP || !parse_create_response(response, status, eid)){
        cerr << "Bad message received from server!" << endl;
        free(Fdata);
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "successful unregister" << endl;
    else if(status == "NOK" || status == "WRP") cout << "incorrect unregister attempt" << endl;
    else if(status == "UNR") cout << "unknown user" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else cerr << "[API] Invalid status message passed through" << endl;
    free(Fdata);
    free(response);
    return 0;       
}
/* ----------------------------------------------- */