#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>   
#include <fcntl.h>      

#include "Client.hpp"
#include "parser.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "../common/io.hpp"
#include "../common/util.hpp"

using namespace std;

#define LINE_SEPARATOR "---------------------------------------"

/* -------------- UDP COMMANDS ------------------- */
int er_login(CLArgs *client, int uid, string pass){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_LOGIN) + " " + 
                        to_string(uid) + " " + pass + "\n";

    if((response = client_udp_request(client, message.c_str())) == NULL){
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
    if(code == INTERNAL_ERROR){
        cerr << "Internal error occured on the server" << endl;
        free(response);
        return -1;
    }
    if(code != OP_LOGIN_RESP || !parse_login_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK"){
        cout << "successful login" << endl;
        client->logged_in = true;
        client->uid = uid;
        client ->pass = pass;
    }
    else if(status == "NOK") cout << "incorrect login attempt" << endl;
    else if(status == "REG"){
        cout << "new user registered" << endl;
        client->logged_in = true;
        client->uid = uid;
        client ->pass = pass;
    }
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

int er_logout(CLArgs *client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_LOGOUT) + " " + to_string(client->uid) + 
                                            " " + client->pass + "\n";
                                            
    if((response = client_udp_request(client, message.c_str())) == NULL){
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
    if(code == INTERNAL_ERROR){
        cerr << "Internal error occured on the server" << endl;
        free(response);
        return -1;
    }
    if(code != OP_LOGOUT_RESP || !parse_logout_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK"){
        cout << "successful logout" << endl;
        client->logged_in = false;
    } 
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

int er_unregister(CLArgs *client){
    char *response, response_code[BUF_TEMP];
    string message = op_to_str(OP_UNREGISTER) + " " + to_string(client->uid) + 
                                            " " + client->pass + "\n";
                                            
    if((response = client_udp_request(client, message.c_str())) == NULL){
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
    if(code == INTERNAL_ERROR){
        cerr << "Internal error occured on the server" << endl;
        free(response);
        return -1;
    }
    if(code != OP_UNREGISTER_RESP || !parse_unregister_response(response, status)){
        cerr << "Bad message received from server!" << endl;
        free(response);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK"){
        cout << "successful unregister" << endl;
        client->logged_in = false;
    }
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

int er_myevents(CLArgs client){
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

void print_myreservations_list(vector<Reservation> reservations_list){
    cout << LINE_SEPARATOR << endl;
    for(auto reservation: reservations_list){
        // Error on server.
        if(reservation.value == -1){
            cout << "ERROR fetching reservation" << endl;
        }
        else{
            cout << "Event id: " << reservation.eid << endl;
            cout << "Reservation date and time: ";
            reservation.datetime.print(true);
            cout << "Places reserved: " << reservation.value << endl;
        }
        cout << LINE_SEPARATOR << endl;
    }
}

int er_myreservations(CLArgs client){
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
    else if(status == "NOK") cout << "User has not made any reservations"
                            << "or all reservation files are corrupted." << endl;
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
    else if(status == "NOE") cout << "event does not exist" << endl;
    else if(status == "EOW") cout << "event was not created by this user" << endl;
    else if(status == "SLD") cout << "event is sold out" << endl;
    else if(status == "PST") cout << "event was in the past" << endl;
    else if(status == "CLO") cout << "event was already closed" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        return -1;
    }
    return 0;       
}

void print_events_list(vector<Event_list> events_list){
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

int er_list(CLArgs client){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_LIST) + "\n";

    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'list' command" << endl;

    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        return -1;
    }
    vector<Event_list> events_list = {};
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

string save_event_file(const Event_Info &event) {
    // Absolute or relative path to api.cpp
    string file = __FILE__;

    // Convert to absolute if relative
    if (!file.empty() && file[0] != '/') {
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd)))
            file = string(cwd) + "/" + file;
    }

    // Remove the filename from the path
    size_t pos = file.find_last_of('/');
    if (pos == string::npos)
        return "";
    file = file.substr(0, pos); 

    // Build path for directory (src/client/Events_Info)
    string dir = file + "/Events_Info";
    mkdir(dir.c_str(), 0755);
    string path = dir + "/" + event.Fname;

    // Create + write file
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return "";
    if (write_all(fd, event.Fdata.data(), event.Fsize) == -1) {
        close(fd);
        return "";
    }

    close(fd);
    return path;
}

void print_event_info(Event_Info &event, const string &filepath) {

    cout << "Event: " << event.name << endl;
    cout << "Date and Time: ";
    event.event_date.print();
    cout << "Total seats: " << event.attendace_size << endl;
    cout << "Reserved seats: " << event.seats_reserved << endl;

    // Determine event status
    string status;
    if (event.seats_reserved >= event.attendace_size)
        status = "sold-out";
    else if (event.event_date.isPast())
        status = "closed";
    else
        status = "open";

    cout << "Status: " << status << "\n";
    cout << "File name: " << event.Fname << "\n";
    cout << "Saved to: " << filepath << "\n";
}

int er_show(CLArgs client, string &eid){
    string message = op_to_str(OP_SHOW) + " " + eid + "\n";
    int fd = client_connect_tcp(client);

    if(fd == -1)
        return -1;
    // Write message.
    if(write_all(fd, message.c_str(), message.size()) == -1){
        cerr << "Failure to write message to server" << endl;
        close(fd);
        return -1;
    }

    // Read code from response
    string response_code = tcp_read_word(fd);
    if(response_code == ""){
        cerr << "Failure to read response code" << endl;
        close(fd);
        return -1;
    }
    // Validate code
    OP_CODE code = str_to_op(response_code.c_str());
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        close(fd);
        return -1;
    }
    // Read and validate status.
    string status = tcp_read_word(fd);
    if(status != "OK" && status != "NOK"){
        cerr << "Invalid status received" << endl;
        close(fd);
        return -1;
    }

    Event_Info event = {};
    if(status == "OK" && !parse_show_response(fd, event)){
        cerr << "Bad message received from server!" << endl;
        close(fd);
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK"){
        string path = save_event_file(event);
        if(path == ""){
            cerr << "Failure to store event file" << endl;
            close(fd);
            return -1;
        }
        print_event_info(event, path);
    }
    else if(status == "NOK") cout << "no file to be sent, event does not exist, or other problems" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        close(fd);
        return -1;
    }
    close(fd);
    return 0;       
}

int er_reserve(CLArgs client, string &eid, int people){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_RESERVE) + " " + to_string(client.uid) + 
                        " " + client.pass + " " + eid + " " + to_string(people) + "\n";
    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'reserve' command" << endl;

    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        return -1;
    }
    int n_seats;
    if(code != OP_RESERVE_RESP || !parse_reserve_response((char*)response.c_str(), status, n_seats)){
        cerr << "Bad message received from server!" << endl;
        return -1;  
    }

    // Print message according to the status value
    if(status == "ACC") cout << "reservation accepted" << endl;
    else if(status == "NOK") cout << "event is not active" << endl;
    else if(status == "NLG") cerr << "[API] user not logged in" << endl;
    else if(status == "CLS") cout << "event is closed" << endl;
    else if(status == "SLD") cout << "reservation refused: event is sold out" << endl;
    else if(status == "REJ"){
        cout << "reservation refused: requested places are larger than remaining seats -> " << n_seats << endl;
    }
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        return -1;
    }
    return 0;       
}

int er_changePass(CLArgs &client, string old_pass, string new_pass){
    char response_code[BUF_TEMP];
    string message = op_to_str(OP_CHANGE_PASS) + " " + to_string(client.uid) +  
                        " " + old_pass + " " + new_pass + "\n";
    string response;
    if((response = client_tcp_request_line(&client, message)) == ""){
        cerr << "Failure to request/receive message to server" << endl;
        return -1;
    }

    // Read response code.
    int n = sscanf(response.c_str(), "%63s", response_code);
    if(n != 1) cerr << "Failure to read response code to 'changePass' command" << endl;

    string status = "";
    OP_CODE code = str_to_op(response_code);
    if(code == ERR){
        cerr << "Invalid request message was sent" << endl;
        return -1;
    }
    if(code != OP_CHANGE_PASS_RESP || !parse_changePass_response((char*)response.c_str(), status)){
        cerr << "Bad message received from server!" << endl;
        return -1;  
    }

    // Print message according to the status value
    if(status == "OK"){
        cout << "successful password change" << endl;
        client.pass = new_pass;
    }
    else if(status == "NOK") cout << "incorrect password" << endl;
    else if(status == "NLG") cerr << "user not logged in" << endl;
    else if(status == "NID") cout << "unknown user" << endl;
    else if(status == "ERR") 
        cerr<< "Syntax of request message is incorrect or parameter values take invalid values" << endl;
    else {
        cerr << "[API] Invalid status message passed through" << endl;
        return -1;
    }
    return 0;       
}
