#include <iostream>
#include <string>
#include <unistd.h>

#include "../common/constants.hpp"
#include "../common/protocol.hpp"
#include "../common/util.hpp"
#include "../common/io.hpp"

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

OP_CODE get_tcp_command(const char *command){
    OP_CODE code = str_to_op(command);
    switch(code){
        case OP_CREATE:
        case OP_CLOSE:
        case OP_LIST:
        case OP_SHOW:
        case OP_RESERVE:
        case OP_CHANGE_PASS:
            return code;
            break;
        default:
            cerr << "Unexpected command received through TCP: " << command << endl;
            return ERR;
    }
}

OP_CODE get_udp_command(const char *command){
    OP_CODE code = str_to_op(command);
    switch(code){
        case OP_LOGIN:
        case OP_LOGOUT:
        case OP_UNREGISTER:
        case OP_MYEVENTS:
        case OP_MYRESERVATIONS:
            return code;
            break;
        default:
            cerr << "Unexpected command received through UDP: " << command << endl;
            return ERR;
    }
}

/* ------------- COMMAND PARSING -------------- */
bool parse_login_request(const char *request, string &uid, string &password){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %255s", code, uid_temp, password_temp, extra);

    if(n != 3 || str_to_op(code) != OP_LOGIN || !is_valid_userid(uid_temp) ||
                                                !is_valid_password(password_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    return true;
}

bool parse_logout_request(const char *request, string &uid, string &password){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %255s", code, uid_temp, password_temp, extra);

    if(n != 3 || str_to_op(code) != OP_LOGOUT || !is_valid_userid(uid_temp) ||
                                                !is_valid_password(password_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    return true;
}

bool parse_unregister_request(const char *request, string &uid, string &password){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %255s", code, uid_temp, password_temp, extra);

    if(n != 3 || str_to_op(code) != OP_UNREGISTER || !is_valid_userid(uid_temp) ||
                                                !is_valid_password(password_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    return true;
}

bool parse_myevents_request(const char *request, string &uid, string &password){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %255s", code, uid_temp, password_temp, extra);

    if(n != 3 || str_to_op(code) != OP_MYEVENTS || !is_valid_userid(uid_temp) ||
                                                !is_valid_password(password_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    return true;
}

bool parse_myreservations_request(const char *request, string &uid, string &password){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %255s", code, uid_temp, password_temp, extra);

    if(n != 3 || str_to_op(code) != OP_MYRESERVATIONS || !is_valid_userid(uid_temp) ||
                                                !is_valid_password(password_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    return true;
}

bool parse_create_request(int fd, const char *request_so_far, 
                            string &uid, string &password, Event_creation_Info &event){
    // On handling of TCP request we already read OP_CODE and UID
    char code[BUF_TEMP], uid_temp[BUF_TEMP], extra[BUFFER_SIZE];

    // Validate OP_CODE and UID.
    int n = sscanf(request_so_far, "%63s %63s %255s", code, uid_temp, extra);
    if(n != 2 || str_to_op(code) != OP_CREATE || !is_valid_userid(uid_temp)){
        return false;
    }

    // Read values one by one and validate.
    string password_temp = tcp_read_word(fd);
    if(!is_valid_password((char *)password_temp.c_str())) return false;
    
    string name = tcp_read_word(fd);
    if(!is_valid_event_name((char *)name.c_str())) return false;

    string date = tcp_read_word(fd);
    if(date == "") return false;
    string time = tcp_read_word(fd);
    if(time == "") return false;
    DateTime dt;
    if(!DateTime::fromStrings(date, time, dt) || dt.isPast()) return false;

    string attendace_size_str = tcp_read_word(fd);
    int attendace_size;
    bool valid = is_positive_integer(attendace_size_str.c_str(), &attendace_size);
    if(!valid || (valid && !is_valid_num_attendees(attendace_size))) return false;

    string Fname = tcp_read_word(fd);
    if(!is_valid_file_name((char *)Fname.c_str())) return false;

    string Fsize_str = tcp_read_word(fd);
    int Fsize;
    valid = is_nonnegative_integer(Fsize_str.c_str(), &Fsize);
    if(!valid || (valid && Fsize >= MAX_FILE_SIZE)) return false;

    // Read file.
    string Fdata;
    Fdata.resize((size_t)Fsize);
    ssize_t size = read_all(fd, &Fdata[0], (size_t)Fsize);
    if(size != Fsize) return false;

    uid = uid_temp;
    password = password_temp;
    event = {name, dt, attendace_size, Fname, (size_t)Fsize, Fdata};
    return true;
}

bool parse_close_request(const char *request, string &uid, string &password, string &eid){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], password_temp[BUF_TEMP];
    char eid_temp[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %63s %63s %63s %255s", code, uid_temp, 
                                                        password_temp, eid_temp, extra);

    if(n != 4 || str_to_op(code) != OP_CLOSE || !is_valid_userid(uid_temp) ||
                                !is_valid_password(password_temp) || !is_valid_eid(eid_temp)){
        return false;
    }

    uid = uid_temp;
    password = password_temp;
    eid = eid_temp;
    return true;
}

bool parse_list_request(const char *request){
    char code[BUF_TEMP], extra[BUFFER_SIZE];
    
    int n = sscanf(request, "%63s %255s", code, extra);
    if(n != 1 || str_to_op(code) != OP_LIST){
        return false;
    }
    return true;
}

bool parse_show_request(void){
    return true;
}

bool parse_reserve_request(const char *request, 
                            string &uid, 
                            string &password, 
                            string &eid,
                            int &people){
    char code[BUF_TEMP], uid_temp[BUF_TEMP], pass_temp[BUF_TEMP];
    char eid_temp[BUF_TEMP], people_temp[BUF_TEMP], extra[BUFFER_SIZE];

    // Validate OP_CODE and UID.
    int n = sscanf(request, "%63s %63s %63s %63s %63s %255s", code, uid_temp, pass_temp,
                                                    eid_temp, people_temp, extra);
    if(n != 5 || str_to_op(code) != OP_RESERVE || 
                        !is_valid_userid(uid_temp) ||
                        !is_valid_password(pass_temp) || 
                        !is_valid_eid(eid_temp) || 
                        !is_valid_seats(people_temp, &people)){
        return false;
    }

    uid = uid_temp;
    password = pass_temp;
    eid = eid_temp;
    return true;
}