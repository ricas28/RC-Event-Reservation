#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "../common/constants.hpp"
#include "../common/util.hpp"
#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"
#include "../common/io.hpp"
#include "commands.hpp"

using namespace std;

#define COMMAND_SIZE 16

/**
 * Parses the received command.
 * 
 * @param buf String with command that will be performed.
 * 
 * @returns Command being performed.
 */
Command get_command(char *buf){
    if(strcmp(buf, "login") == 0) return CMD_LOGIN;
    else if(strcmp(buf, "changePass") == 0) return CMD_CHANGE_PASS;
    else if(strcmp(buf, "unregister") == 0) return CMD_UNREGISTER;
    else if(strcmp(buf, "logout") == 0) return CMD_LOGOUT;
    else if(strcmp(buf, "exit") == 0) return CMD_EXIT;
    else if(strcmp(buf, "create") == 0) return CMD_CREATE;
    else if(strcmp(buf, "close") == 0) return CMD_CLOSE;
    else if(strcmp(buf, "mye") == 0 || strcmp(buf, "myevents") == 0) return CMD_MYEVENTS;
    else if(strcmp(buf, "list") == 0) return CMD_LIST;
    else if(strcmp(buf, "show") == 0) return CMD_SHOW;
    else if(strcmp(buf, "reserve") == 0) return CMD_RESERVE;
    else if(strcmp(buf, "myr") == 0 || strcmp(buf, "myreservations") == 0) return CMD_MYRESERVATIONS;
    else return CMD_INVALID;
}

bool parse_args(string &port, string &ip, char** argv, int argc){
    int  opt;

    // Max number of arguments is 5.
    if(argc > 5)
        return false;

    // Put default argument values in case they are not present.
    ip = DEFAULT_IP;
    port = DEFAULT_PORT;

    // opt will be option on argv and optarg is the next item on argv.
    while ((opt = getopt(argc, argv, "n:p:")) != -1) {
        switch (opt) {
            case 'n':
                ip = optarg;   
                break;
            case 'p':
                port = optarg;
                break;
            // Unknown command (neither -n nor -p)
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

Command parse_command(char *line, char **args){
    char command[COMMAND_SIZE];
    if (sscanf(line, "%15s", command) < 1) {
        cout << "Failure to parse command!" << endl;
        return CMD_INVALID;
    }

    size_t command_size = strlen(command);
    // Move pointer past the command
    char* ptr = line + command_size;
    // Skip all spaces
    while (*ptr && isspace(*ptr) ) ptr++;
    if (*ptr) {
        *args = ptr;
        // Remove newline at the end, if present
        size_t len = strlen(*args);
        if (len > 0 && (*args)[len-1] == '\n')
            (*args)[len-1] = '\0';
    } 
    else {
        *args = NULL;
    }

    return get_command(command);
}

/* ------------------------------------------------------------------------- */

bool parse_login(char *args, int *uid, string *pass){
    bool error = false;
    char uid_temp[BUF_TEMP], pass_temp[BUF_TEMP], extra[BUFFER_SIZE];

    if(!args){
        cout << "No arguments were given!\nUsage: login <userID> <password>" << endl; 
        return false;
    }
    int n = sscanf(args, "%63s %63s %255s", uid_temp, pass_temp, extra);
    if (n != 2) {
        cout << "Invalid arguments!" << endl;
        cout << "Usage: login <userID> <password>" << endl;
        return false;
    }
    if (!is_valid_userid(uid_temp)) {
        cout << "Invalid userID! Must be exactly 6 digits." << endl;
        error = true;
    }
    if (!is_valid_password(pass_temp)) {
        cout << "Invalid password! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if (error) return false;
    // Successful parse.
    *uid = atoi(uid_temp);
    *pass = pass_temp;

    return true;
}

bool parse_login_response(char *response, string &status){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
    // Response has 2 arguments, has code OP_LOGIN_RESP, status and ends with '\n'.
    if(n != 2 || str_to_op(response_code) != OP_LOGIN_RESP || response[strlen(response)-1] != '\n'){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK") || !strcmp(status_temp, "NOK") || 
        !strcmp(status_temp, "REG") || !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_changePass(char *args, string *old_pass, string *new_pass){
    bool error = false;
    char old_pass_temp[BUF_TEMP], new_pass_temp[BUF_TEMP], extra[BUFFER_SIZE];

    if(!args){
        cout << "No arguments were given!\nUsage: changePass <oldPassword> <newPassword>" << endl; 
        return false;
    }
    int n = sscanf(args, "%63s %63s %255s", old_pass_temp, new_pass_temp, extra);
    if (n != 2) {
        cout << "Invalid arguments!" << endl;
        cout << "Usage: changePass <oldPassword> <newPassword>" << endl;
        return false;
    }
    if (!is_valid_password(old_pass_temp)) {
        cout << "Old password is invalid! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if (!is_valid_password(new_pass_temp)) {
        cout << "New password is invalid! Must be exactly 8 alphanumeric characters." << endl;
        error = true;
    }
    if(strcmp(old_pass_temp, new_pass_temp) == 0){
        cout << "Passwords can't be equal!" << endl;
        error = true;
    }
    if (error) return false;
    // Successful parse.
    *old_pass = old_pass_temp;
    *new_pass = new_pass_temp;
    return true; 
}

bool parse_changePass_response(char *response, string &status){
     char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
    // Response has 2 arguments, has code OP_CHANGE_PASS_RESP, status and ends with '\n'.
    if(n != 2 || str_to_op(response_code) != OP_CHANGE_PASS_RESP || response[strlen(response)-1] != '\n'){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK") || !strcmp(status_temp, "NOK") || 
        !strcmp(status_temp, "NID") || !strcmp(status_temp, "NLG") || 
        !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}


/** --- No arguments commands --- */
bool parse_unregister(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: unregister" << endl;
        return false;
    }
    return true;
}

bool parse_unregister_response(char *response, string &status){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
    // Response has 2 arguments: code OP_UNREGISTER_RESP, status and ends with '\n'.
    if(n != 2 || str_to_op(response_code) != OP_UNREGISTER_RESP || response[strlen(response)-1] != '\n'){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK") || !strcmp(status_temp, "NOK") || 
        !strcmp(status_temp, "UNR") || !strcmp(status_temp, "WRP") || 
        !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_logout(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: logout" << endl;
        return false;
    }
    return true;
}

bool parse_logout_response(char *response, string &status){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
    // Response has 2 arguments: code OP_LOGOUT_RESP, status and ends with '\n'.
    if(n != 2 || str_to_op(response_code) != OP_LOGOUT_RESP || response[strlen(response)-1] != '\n'){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK") || !strcmp(status_temp, "NOK") || 
        !strcmp(status_temp, "UNR") || !strcmp(status_temp, "WRP") || 
        !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_exit(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: exit" << endl;
        return false;
    }
    return true;
}

bool parse_myevents(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: myevents (or) mye" << endl;
        return false;
    }
    return true;
}

bool parse_myevents_list(char *response, vector<pair<string, int>> &events_list){
    string s(response);
    istringstream iss(s);

    string token;

    // Skip the response code and status in the response.
    iss >> token; // response_code
    iss >> token; // status

    // Read in pairs of EID and state.
    char eid[BUF_TEMP];
    int state;
    while (iss >> eid >> state) {
        if(state != EVENT_CORRUPTED){
            // Validate EID
            if (!is_valid_eid(eid)){
                cerr << "Invalid EID on server's response: " << eid << endl;  
                return false;
            }
            // Validate state
            if (!is_valid_event_state(state)){
                cerr << "Invalid event state on server's response: " << state << endl; 
                return false;
            }
        }
        events_list.push_back({eid, state});
    }
    return true;
}

bool parse_myevents_response(char *response, string &status, vector<pair<string, int>> &events_list){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s", response_code, status_temp);
    // Response starts with 2 arguments: code OP_MYEVENTS_RESP, status.
    if(n != 2 || str_to_op(response_code) != OP_MYEVENTS_RESP){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK")){
        if(!parse_myevents_list(response, events_list)){
            return false;
        }
        status = status_temp;
        return true;
    }

    if(!strcmp(status_temp, "NOK") || !strcmp(status_temp, "NLG") || 
        !strcmp(status_temp, "WRP") || !strcmp(status_temp, "ERR")){
            n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
            if(n != 2 && response[strlen(response)-1] != '\n')
                return false;
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_list(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: list" << endl;
        return false;
    }
    return true;
}

bool parse_events_list(const char *response, vector<Event_list> &events_list){
    string s(response);
    istringstream iss(s);

    string token;
    iss >> token; // "RLS"
    iss >> token; // status (OK, NOK, WRP, NLG)

    // If token is not OK, there will be no list.
    if (token != "OK") {
        return true;
    }

    string eid;
    string name;
    int state;
    string datepart;   // dd-mm-yyyy
    string timepart;   // hh:mm

    while (iss >> eid >> name >> state >> datepart >> timepart){
        // Check for corrupted
        if(state == EVENT_CORRUPTED){
            events_list.push_back({eid, "", EVENT_CORRUPTED, DateTime()});
            continue;
        }
        // Validate EID
        if (!is_valid_eid((char *)eid.c_str())) {
            cerr << "Invalid EID in server response: " << eid << endl;
            return false;
        }
        // Validate event name
        if (!is_valid_event_name((char*)name.c_str())) {
            cerr << "Invalid event name in server response: " << name << endl;
            return false;
        }
        // Validate event state
        if(!is_valid_event_state(state)){
            cerr << "Invalid event state in server response: " << state << endl;
            return false;
        }
        // Validate date and time.
        DateTime dt;
        if (!DateTime::fromStrings(datepart, timepart, dt)) {
            // Method already prints error messages.
            return false; 
        }
        events_list.push_back({eid, name, state, dt});
    }
    return true;
}

bool parse_list_response(const char *response, string &status, vector<Event_list> &events_list){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s", response_code, status_temp);
    // Response starts with 2 arguments: code OP_LIST_RESP, status.
    if(n != 2 || str_to_op(response_code) != OP_LIST_RESP){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK")){
        if(!parse_events_list(response, events_list)){
            return false;
        }
        status = status_temp;
        return true;
    }

    if(!strcmp(status_temp, "NOK") || !strcmp(status_temp, "ERR")){
            n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
            if(n != 2)
                return false;
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_myreservations(char *args){
    if (args != NULL) {
        cout << "This command has no arguments!" << endl;
        cout << "Usage: myreservations (or) myr" << endl;
        return false;
    }
    return true;
}

bool parse_reservations_list(char *response, vector<Reservation> &reservations){
    string s(response);
    istringstream iss(s);

    string token;
    iss >> token; // "RMR"
    iss >> token; // status (OK, NOK, WRP, NLG)

    // If token is not OK, there will be no list.
    if (token != "OK") {
        return true;
    }

    string eid;
    string datepart;   // dd-mm-yyyy
    string timepart;   // hh:mm
    int value;

    while (iss >> eid >> datepart >> timepart >> value){
        // Check for corrupted reservation from server.
        if(eid == "-1" && datepart == "00-00-0000" && timepart == "00:00:00" && value == -1){
            reservations.push_back({eid, DateTime(), value});
            continue;
        }

        // Validate EID
        if (!is_valid_eid((char *)eid.c_str())) {
            cerr << "Invalid EID in server response: " << eid << endl;
            return false;
        }
        // Validate value
        if (value < 1 || value > 999) {
            cerr << "Invalid reservation value in server response: " << value << endl;
            return false;
        }
        // Validate date and time
        DateTime dt;
        if (!DateTime::fromStrings(datepart, timepart, dt)) {
            // Method already prints error messages.
            return false; 
        }
        reservations.push_back({eid, dt, value});
    }
    return true;
}

bool parse_myreservations_response(char *response, string &status, vector<Reservation> &reservations_list){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s", response_code, status_temp);
    // Response starts with 2 arguments: code OP_MYRESERVATIONS_RESP, status.
    if(n != 2 || str_to_op(response_code) != OP_MYRESERVATIONS_RESP){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK")){
        if(!parse_reservations_list(response, reservations_list)){
            return false;
        }
        status = status_temp;
        return true;
    }

    if(!strcmp(status_temp, "NOK") || !strcmp(status_temp, "NLG") || 
        !strcmp(status_temp, "WRP") || !strcmp(status_temp, "ERR")){
            n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
            if(n != 2 && response[strlen(response)-1] != '\n')
                return false;
            status = status_temp;
            return true;
    }
    return false;
}
/* ----------------------------------*/

bool parse_create(char *args, string *name, string *event_fname, ssize_t *Fsize,
                        DateTime *event_date, int *num_attendees){
    char name_temp[BUF_TEMP], event_fname_temp[BUF_TEMP], extra[BUFFER_SIZE];
    int day, month, year, hour, minute, num_attendees_temp;

    if(!args){
        cout << "No arguments were given!\nUsage: create <name> <event_fname> ";
        cout << "<event_date> (format dd-mm-yyyy hh:mm) <num_attendees>" << endl;
        return false;
    }
    int n = sscanf(args, "%63s %63s %d-%d-%d %d:%d %d %255s", name_temp, event_fname_temp, 
                                                &day, &month, &year, &hour, &minute,
                                                &num_attendees_temp, extra);
    if(n != 8){
        cout << "Invalid arguments!" << endl;
        cout << "Usage: create <name> <event_fname> ";
        cout << "<event_date> (format dd-mm-yyyy hh:mm) <num_attendees>" << endl;
        return false;
    }
    if(!is_valid_event_name(name_temp)){
        cout << "Invalid event name! Must be up to 10 alphanumeric characters." << endl;
        return false;
    }
    if(!is_valid_file_name(event_fname_temp)){
        cout << "Invalid file name! Must be up to 24 alphanumeric characters ";
        cout << "(plus '-', '_' and '.'), including the 3-letter extension." << endl;
        return false;
    }
    if(!file_exists(event_fname_temp)){
        cout << "Invalid file! File does not exist or does not exist on the current folder." << endl;
        return false;
    }
    if(!is_valid_date_time(day, month, year, hour, minute)){
        cout << "Invalid date and time!";
        cout << " Date and time is invalid or before current date and time." << endl;
        return false;
    }
    if(!is_valid_num_attendees(num_attendees_temp)){
        cout << "Invalid number of attendees! Must be number from 10 to 999." << endl;
        return false;
    }
    // Read file and see if exceeds max_file_size.
    size_t file_size;
    if(!get_file_size(event_fname_temp, &file_size)){
        cout << "Invalid file!" << endl;
        return false;
    }
    if(file_size > MAX_FILE_SIZE){
        cout << "File is too large for the server." << endl;
        return false;
    }
    // Successful parse.
    *name = name_temp;
    *event_fname = event_fname_temp;
    *event_date = DateTime(day, month, year, hour, minute);
    *num_attendees = num_attendees_temp;
    *Fsize = (ssize_t)file_size;
    return true;
}

bool parse_create_response(const char *response, string &status, string &eid){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], eid_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s", response_code, status_temp);
    // Response starts with 2 arguments: code OP_LOGOUT_RESP and status.
    if(n != 2 || str_to_op(response_code) != OP_CREATE_RESP){
       return false;
    }

    if(!strcmp(status_temp, "OK")){
        n = sscanf(response, "%63s %63s %63s %255s", 
                            response_code, status_temp, eid_temp, extra);
        // Response needs to have eid.
        if(n != 3 || !is_valid_eid(eid_temp)){
            return false;
        }
        status = status_temp;
        eid = eid_temp;
        return true;
    }
    // Check for status value
    if(!strcmp(status_temp, "NOK") || !strcmp(status_temp, "NLG") ||
        !strcmp(status_temp, "WRP") || !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_close(char *args, string &eid){
    char eid_temp[BUF_TEMP], extra[BUFFER_SIZE];

    if(!args){
        cout << "No arguments were given!\nUsage: close <EID>" << endl; 
        return false;
    }
    int n = sscanf(args, "%63s %255s", eid_temp, extra);
    if(n != 1){
        cout << "Invalid arguments!\nUsage: close <EID>" << endl;
        return false;
    }
    if(!is_valid_eid(eid_temp)){
        cout << "Invalid eventID! Must be a 3 number digit." << endl;
        return false;
    }
    // Successful parse.
    eid = eid_temp;
    return true;
}

bool parse_close_response(const char *response, string &status){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s %255s", response_code, status_temp, extra);
    // Response has 2 arguments: code OP_CLOSE_RESP, status.
    // Reading from TCP already checks for '\n'.
    if(n != 2 || str_to_op(response_code) != OP_CLOSE_RESP){
       return false;
    }

    // Check for status value
    if(!strcmp(status_temp, "OK") || !strcmp(status_temp, "NOK") || 
        !strcmp(status_temp, "NLG") || !strcmp(status_temp, "NOE") ||
        !strcmp(status_temp, "EOW") || !strcmp(status_temp, "SLD") ||
        !strcmp(status_temp, "PST") || !strcmp(status_temp, "CLO") ||
        !strcmp(status_temp, "ERR")){
            status = status_temp;
            return true;
    }
    return false;
}

bool parse_show(char *args, string &eid){
    char eid_temp[BUF_TEMP], extra[BUFFER_SIZE];

    if(!args){
        cout << "No arguments were given!\nUsage: show <EID>" << endl; 
        return false;
    }
    int n = sscanf(args, "%63s %255s", eid_temp, extra);
    if(n != 1){
        cout << "Invalid arguments!\nUsage: show <EID>" << endl;
        return false;
    }
    if(!is_valid_eid(eid_temp)){
        cout << "Invalid eventID! Must be a 3 number digit." << endl;
        return false;
    }
    // Successful parse.
    eid = eid_temp;
    return true;
}

bool parse_show_response(int fd, Event_show_Info &event){
    // Read and validate UID.
    string uid = tcp_read_word(fd);
    if(!is_valid_userid((char *)uid.c_str())){
        cerr << "Invalid user id received" << endl;
        return false;
    }

    // Read and validate event name.
    string name = tcp_read_word(fd);
    if(!is_valid_event_name((char *)name.c_str())){
        cerr << "Invalid event name received" << endl;
        return false;
    }

    // Read and validate date and time.
    string date = tcp_read_word(fd);
    string time = tcp_read_word(fd);
    DateTime dt;
    if(!DateTime::fromStrings(date, time, dt))
        // fromStrings method already prints error messages.
        return false;

    // Read and validate attendace_size.
    string attendace_size_temp = tcp_read_word(fd);
    int attendace_size;
    bool valid_integer = is_positive_integer(attendace_size_temp.c_str(), &attendace_size);
    if(!valid_integer || (valid_integer && !is_valid_num_attendees(attendace_size))){
        cerr << "Invalid attendace size received" << endl;
        return false;
    }

    // Read and validate seats_reserved.
    string seats_reserved_temp = tcp_read_word(fd);
    int seats_reserved;
    valid_integer = is_nonnegative_integer((char *)seats_reserved_temp.c_str(), &seats_reserved);
    if(!valid_integer || (valid_integer && seats_reserved > attendace_size)){
        cerr << "Invalid reserved seats received" << endl;
        return false;
    }

    // Read and validate file name.
    string Fname = tcp_read_word(fd);
    if(!is_valid_file_name((char *)Fname.c_str())){
        cerr << "Invalid file name received" << endl;
        return false;
    }

    // Read and validate Fsize.
    string Fsize_temp = tcp_read_word(fd);
    int Fsize;
    valid_integer = is_nonnegative_integer(Fsize_temp.c_str(), &Fsize); 
    if(!valid_integer || (valid_integer && Fsize > MAX_FILE_SIZE)){
        cerr << "Invalid file size received" << endl;
        return false;
    }

    // Initialize struct.
    event = {stoi(uid), name, dt, attendace_size, seats_reserved, Fname, (size_t)Fsize};
    return true;
}

bool parse_reserve(char *args, string &eid, int &seats){
    char eid_temp[BUF_TEMP], seats_temp[BUF_TEMP], extra[BUFFER_SIZE];
    bool error = false;
    
    if(!args){
        cout << "No arguments were given!\nUsage: reserve <EID> <value>" << endl; 
        return false;
    }
    int n = sscanf(args, "%63s %63s %255s", eid_temp, seats_temp, extra);
    if(n != 2){
        cout << "Invalid arguments!\nUsage: reserve <EID> <value>" << endl;
        return false;
    }
    if(!is_valid_eid(eid_temp)){
        cout << "Invalid eventID! Must be a 3 number digit." << endl;
        error = true;
    }
    if(!is_valid_seats(seats_temp)){
        cout << "Invalid number of seats! Must be an integer from 1 to " << MAX_SEATS << endl;
        error = true;
    }
    if (error) return false;
    // Successful parse.
    eid = eid_temp;
    seats = atoi(seats_temp);
    return true;
}

bool parse_reserve_response(char *response, string &status, int &n_seats){
    char response_code[BUF_TEMP], status_temp[BUF_TEMP];
    char n_seats_temp[BUFFER_SIZE], extra[BUFFER_SIZE];

    int n = sscanf(response, "%63s %63s", response_code, status_temp);
    // Response starts with 2 arguments: code OP_RESERVE_RESP, status.
    if(n != 2 || str_to_op(response_code) != OP_RESERVE_RESP){
       return false;
    }

    // Check for 'REJ' status.
    if(!strcmp(status_temp, "REJ")){
        n = sscanf(response,  "%63s %63s %63s %255s", response_code, status_temp,
                                                            n_seats_temp, extra);
        // Reading from TCP already garantees that message ends with '\n'.
        if(n != 3 || !is_valid_seats(n_seats_temp))
            return false;
        n_seats = atoi(n_seats_temp);
        status = status_temp;
        return true;
    }

    // Check for status value
    if(!strcmp(status_temp, "NOK") || !strcmp(status_temp, "NLG") || 
        !strcmp(status_temp, "ACC") || !strcmp(status_temp, "CLS") ||
        !strcmp(status_temp, "SLD") || !strcmp(status_temp, "ERR") ||
        !strcmp(status_temp, "PST") || !strcmp(status_temp, "WRP")){
            status = status_temp;
            return true;
    }
    return false;
}
