#include <iostream>

#include "../common/protocol.hpp"

using namespace std;

void handle_login(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "LOGIN" << endl;
}

void handle_logout(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "LOGOUT" << endl;
}

void handle_unregister(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "UNR" << endl;
}

void handle_myevents(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "MYE" << endl;
}

void handle_myreservations(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "MYR" << endl;
}

void handle_create(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "CREATE" << endl;
}

void handle_close(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "CLOSE" << endl;
}

void handle_list(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "LIST" << endl;
}

void handle_show(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "SHOW" << endl;
}

void handle_reserve(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "RESERVE" << endl;
}

void handle_changePass(int fd, const char *request){
    (void)fd;
    (void)request;
    cout << "CHANGE" << endl;
}

void handle_request(int fd, OP_CODE code, const char *request){
    if(code == OP_LOGIN) 
        handle_login(fd, request);
    else if(code == OP_LOGOUT) 
        handle_logout(fd, request);
    else if(code == OP_UNREGISTER) 
        handle_unregister(fd, request);
    else if(code == OP_MYEVENTS) 
        handle_myevents(fd, request);
    else if(code == OP_MYRESERVATIONS)
        handle_myreservations(fd, request);
    else if(code == OP_CREATE) 
        handle_create(fd, request);
    else if(code == OP_CLOSE) 
        handle_close(fd, request);
    else if(code == OP_LIST) 
        handle_list(fd, request);
    else if(code == OP_SHOW) 
        handle_show(fd, request);
    else if(code == OP_RESERVE) 
        handle_reserve(fd, request);
    else if(code == OP_CHANGE_PASS) 
        handle_changePass(fd, request);
    else cerr << "Invalid OP_CODE passes through";       
}