#include <iostream>

#include "Client.hpp"
#include "parser.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "../common/util.hpp"

using namespace std;

#define LINE_SEPARATOR "---------------------------------------"

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
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(response);
        return -1;
    }
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
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(response);
        return -1;
    }
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
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(response);
        return -1;
    }
    free(response);
    return 0;       
}

void print_myevents_list(vector<pair<string, int>> &events_list){
    cout << LINE_SEPARATOR << endl;
    for(auto event : events_list){
        cout << "Event id: " << event.first << endl;
        cout << "Event status: ";
        print_event_state(event.second);
        cout << LINE_SEPARATOR << endl;
    }
}

int er_myevents(ClLArgs client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_MYEVENTS) + " " + to_string(client.uid) + 
                                            " " + client.pass + "\n";
                                            
    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'myevents' command" << endl;
    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(response);
        return -1;
    }
    vector<pair<string, int>> events_list = {};
    if(code != OP_MYEVENTS_RESP || !parse_myevents_response(response, status, events_list)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") print_myevents_list(events_list);
    else if(status == "NOK") cout << "user has not created any events" << endl;
    else if(status == "NLG") cerr << "user not logged in" << endl;
    else if(status == "WRP") cerr << "[API] wrong password sent on the request" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(response);
        return -1;
    }
    free(response);
    return 0;  
}

void print_myreservations_list(vector<Reservation> reservations_list){
    cout << LINE_SEPARATOR << endl;
    for(auto reservation: reservations_list){
        cout << "Event id: " << reservation.eid << endl;
        cout << "Event date and time: ";
        reservation.datetime.print();
        cout << "Places reserved: " << reservation.value << endl;
        cout << LINE_SEPARATOR << endl;
    }
}

int er_myreservations(ClLArgs client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_MYRESERVATIONS) + " " + to_string(client.uid) + 
                                            " " + client.pass + "\n";
                                            
    if((response = client_udp_request(&client, message.c_str())) == NULL){
        cerr << "Failure to request/receive message to server" << endl;
        free(response);
        return -1;
    }
    // Read response code.
    int n = sscanf(response, "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'myreservations' command" << endl;
    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(response);
        return -1;
    }
    vector<Reservation> reservations_list = {};
    if(code != OP_MYRESERVATIONS_RESP || 
            !parse_myreservations_response(response, status, reservations_list)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") print_myreservations_list(reservations_list);
    else if(status == "NOK") cout << "user has not made any reservations" << endl;
    else if(status == "NLG") cerr << "[API] user not logged in" << endl;
    else if(status == "WRP") cerr << "[API] wrong password sent on the request" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(response);
        return -1;
    }
    free(response);
    return 0;  
}

/* ----------------------------------------------- */

/* -------------- TCP COMMANDS ------------------- */
int er_create(CLArgs client, string name, string event_fname, size_t Fsize,
                        char *Fdata, DateTime event_date, int num_attendees){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_CREATE) + " " + to_string(client.uid) + " " + client.pass +
                        " " + name + " " + event_date.toString() + " " + to_string(num_attendees) +
                        " " + event_fname + " " + to_string(Fsize) + " " + Fdata + "\n";
    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        free(Fdata);
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'create' command" << endl;

    string status = "", eid = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        free(Fdata);
        return -1;
    }
    if(code != OP_CREATE_RESP || !parse_create_response(response.c_str(), status, eid)){
        cerr << "Bad message received from server!" << endl;
        free(Fdata);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "successful create\nevent id: " << eid << endl;
    else if(status == "NLG") cerr << "[API] user not logged in" << endl;
    else if(status == "WRP") cerr << "[API] wrong password was sent on the request" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        free(Fdata);
        return -1;
    }
    free(Fdata);
    return 0;       
}

int er_close(CLArgs client, string eid){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_CLOSE) + " " + to_string(client.uid) + 
                        " " + client.pass + " " + eid + "\n";
    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'close' command" << endl;

    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        return -1;
    }
    if(code != OP_CLOSE_RESP || !parse_close_response(response.c_str(), status)){
        cerr << "Bad message received from server!" << endl;
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") cout << "event successfuly closed" << endl;
    else if(status == "NOK") cout << "[API] user does not exist or password is incorrect" << endl;
    else if(status == "NLG") cerr << "[API] user not logged in" << endl;
    else if(status == "NOE") cerr << "event does not exist" << endl;
    else if(status == "EOW") cerr << "event was not created by this user" << endl;
    else if(status == "SLD") cerr << "event is sold out" << endl;
    else if(status == "PST") cerr << "event was in the past" << endl;
    else if(status == "CLO") cerr << "event was already closed" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        return -1;
    }
    return 0;       
}

void print_events_list(vector<Event> events_list){
    cout << LINE_SEPARATOR << endl;
    for(auto event: events_list){
        cout << "Event id: " << event.eid << endl;
        cout << "Event name: " << event.name << endl;
        cout << "Event status: ";
        print_event_state(event.state);
        cout << "Event date and time: ";
        event.datetime.print();
        cout << LINE_SEPARATOR << endl;
    }
}

int er_list(ClLArgs client){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_LIST) + "\n";

    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'close' command" << endl;

    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        return -1;
    }
    vector<Event> events_list = {};
    if(code != OP_LIST_RESP || !parse_list_response(response.c_str(), status, events_list)){
        cerr << "Bad message received from server!" << endl;
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK") print_events_list(events_list);
    else if(status == "NOK") cout << "no events were created" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        return -1;
    }
    return 0;      
}
/* ----------------------------------------------- */