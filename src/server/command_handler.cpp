#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../common/protocol.hpp"
#include "command_handler.hpp"
#include "parser.hpp"

using namespace std;

void handle_login(UDPSender &sender, const char *request){
    string uid, password;

    if(!parse_login_request(request, uid, password)){
        string message = op_to_str(OP_LOGIN_RESP) + " ERR\n";
        if(send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr,
                                                                            sender.addrlen) == -1){
            cerr << "Failure to send error message to client" << endl;
        }
        return;
    }
    cout << uid << endl << password << endl;
    send_udp_message(sender.fd, "TESTEEEEEEEEEEE", (struct sockaddr*)&sender.client_addr,
                                                                            sender.addrlen);
    /*
    if(login() == -1)
        cerr << "Failure to execute login command" << endl;
    */
}

void handle_logout(UDPSender sender, const char *request){
    (void)sender;
    (void)request;
    cout << "LOGOUT" << endl;
}

void handle_unregister(UDPSender sender, const char *request){
    (void)sender;
    (void)request;
    cout << "UNR" << endl;
}

void handle_myevents(UDPSender sender, const char *request){
    (void)sender;
    (void)request;
    cout << "MYE" << endl;
}

void handle_myreservations(UDPSender sender, const char *request){
    (void)sender;
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

void process_UDP_request(UDPSender sender, OP_CODE code, const char *request){
    if(code == OP_LOGIN) 
        handle_login(sender, request);
    else if(code == OP_LOGOUT) 
        handle_logout(sender, request);
    else if(code == OP_UNREGISTER) 
        handle_unregister(sender, request);
    else if(code == OP_MYEVENTS) 
        handle_myevents(sender, request);
    else if(code == OP_MYRESERVATIONS)
        handle_myreservations(sender, request);
    else cerr << "[HANDLER] Invalid OP_CODE passed through TCP";       
}

void process_TCP_request(int fd, OP_CODE code, const char *request){
    if(code == OP_CREATE) 
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
    else cerr << "[HANDLER] Invalid OP_CODE passed through TCP";    
}