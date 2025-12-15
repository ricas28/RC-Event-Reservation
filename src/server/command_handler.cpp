#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <utility>
#include <sstream>

#include "../common/protocol.hpp"
#include "../common/io.hpp"
#include "../common/constants.hpp"
#include "command_handler.hpp"
#include "parser.hpp"
#include "operations.hpp"

using namespace std;

void handle_login(UDPSender &sender, const char *request){
    string uid, password;

    if(!parse_login_request(request, uid, password)){
        string message = op_to_str(OP_LOGIN_RESP) + " ERR\n";
        send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
        return;
    }
    
    string message;
    switch(login(uid, password)){
        case LoginResult::IO_ERROR:
            cerr << "Failute to execute 'login' command" << endl;
            send_udp_message(sender.fd, "NOK\n", (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LoginResult::SUCCESS:
            message = op_to_str(OP_LOGIN_RESP) + " OK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LoginResult::REGISTERED:
            message = op_to_str(OP_LOGIN_RESP) + " REG\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LoginResult::WRONG_PASS:
            message = op_to_str(OP_LOGIN_RESP) + " NOK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
    }   
}

void handle_logout(UDPSender sender, const char *request){
    string uid, password;

    if(!parse_logout_request(request, uid, password)){
        string message = op_to_str(OP_LOGOUT_RESP) + " ERR\n";
        send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
        return;
    }
    
    string message;
    switch(logout(uid, password)){
        case LogoutResult::IO_ERROR:
            cerr << "Failute to execute 'login' command" << endl;
            send_udp_message(sender.fd, "NOK\n", (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LogoutResult::SUCCESS:
            message = op_to_str(OP_LOGOUT_RESP) + " OK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LogoutResult::WRONG_PASS:
            message = op_to_str(OP_LOGOUT_RESP) + " WRP\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LogoutResult::NOT_LOGGED_IN:
            message = op_to_str(OP_LOGOUT_RESP) + " NOK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case LogoutResult::NOT_REGISTERED:
            message = op_to_str(OP_LOGOUT_RESP) + " UNR\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
    }   
}

void handle_unregister(UDPSender sender, const char *request){
    string uid, password;

    if(!parse_unregister_request(request, uid, password)){
        string message = op_to_str(OP_UNREGISTER_RESP) + " ERR\n";
        send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
        return;
    }
    
    string message;
    switch(unregister(uid, password)){
        case UnregisterResult::IO_ERROR:
            cerr << "Failute to execute 'login' command" << endl;
            send_udp_message(sender.fd, "NOK\n", (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case UnregisterResult::SUCCESS:
            message = op_to_str(OP_UNREGISTER_RESP) + " OK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case UnregisterResult::WRONG_PASS:
            message = op_to_str(OP_UNREGISTER_RESP) + " WRP\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case UnregisterResult::NOT_LOGGED_IN:
            message = op_to_str(OP_UNREGISTER_RESP) + " NOK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case UnregisterResult::NOT_REGISTERED:
            message = op_to_str(OP_UNREGISTER_RESP) + " UNR\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
    }   
}

string events_to_string(const vector<pair<string,int>> &events) {
    ostringstream oss;
    bool first = true;

    for (const auto &e : events) {
        if (!first) oss << " ";
        oss << e.first << " " << e.second; 
        first = false;
    }

    return oss.str();
}

void handle_myevents(UDPSender sender, const char *request){
    string uid, password;

    if(!parse_myevents_request(request, uid, password)){
        string message = op_to_str(OP_MYEVENTS_RESP) + " ERR\n";
        send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
        return;
    }
    
    vector<pair<string, int>> events;
    string message;
    switch(myevents(uid, password, events)){
        case MyEventsResult::SUCCESS:
            message = op_to_str(OP_MYEVENTS_RESP) + " OK " + events_to_string(events) + "\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyEventsResult::WRONG_PASS:
            message = op_to_str(OP_MYEVENTS_RESP) + " WRP\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyEventsResult::NOT_LOGGED_IN:
            message = op_to_str(OP_MYEVENTS_RESP) + " NLG\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyEventsResult::NO_EVENTS_CREATED:
            message = op_to_str(OP_MYEVENTS_RESP) + " NOK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
    }   
}

string reservations_to_string(vector<Reservation> reservations){
    ostringstream oss;
    bool first = true;

    for (auto &r : reservations) {
        if (!first) oss << " ";
        oss << r.eid << " " << r.datetime.toString(true) << " " << r.value; 
        first = false;
    }

    return oss.str();
}

void handle_myreservations(UDPSender sender, const char *request){
    string uid, password;

    if(!parse_myreservations_request(request, uid, password)){
        string message = op_to_str(OP_MYRESERVATIONS_RESP) + " ERR\n";
        send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
        return;
    }
    
    vector<Reservation> reservations;
    string message;
    switch(myreservations(uid, password, reservations)){
        case MyReservationsResult::SUCCESS:
            message = op_to_str(OP_MYRESERVATIONS_RESP) + " OK " + reservations_to_string(reservations) + "\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyReservationsResult::WRONG_PASS:
            message = op_to_str(OP_MYRESERVATIONS_RESP) + " WRP\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyReservationsResult::NOT_LOGGED_IN:
            message = op_to_str(OP_MYRESERVATIONS_RESP) + " NLG\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
        case MyReservationsResult::NO_RESERVATIONS_MADE:
            message = op_to_str(OP_MYRESERVATIONS_RESP) + " NOK\n";
            send_udp_message(sender.fd, message.c_str(), (struct sockaddr*)&sender.client_addr, sender.addrlen);
            return;
    }   
}

void handle_create(int fd, string request_so_far){
    string uid, password;
    Event_creation_Info event;

    if(!parse_create_request(fd, request_so_far.c_str(), uid, password, event)){
        string message = op_to_str(OP_CREATE_RESP) + " ERR\n";
        write_all(fd, message.c_str(), message.size());
        return;
    }
    
    string message, eid;
    switch(create(uid, password, event, eid)){
        case CreateResult::SUCCESS:
            message = op_to_str(OP_CREATE_RESP) + " OK " + eid + "\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CreateResult::WRONG_PASS:
            message = op_to_str(OP_CREATE_RESP) + " WRP\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CreateResult::NOT_LOGGED_IN:
            message = op_to_str(OP_CREATE_RESP) + " NLG\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CreateResult::FAILED_CREATE:
            message = op_to_str(OP_CREATE_RESP) + " NOK\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CreateResult::IO_ERROR:
            write_all(fd, "NOK\n", 4);
            return;
    }  
}

void handle_close(int fd, string request_so_far){
    string uid, password, eid;
    
    string request = request_so_far + tcp_read_message(fd);
    if(!parse_close_request(request.c_str(), uid, password, eid)){
        string message = op_to_str(OP_CLOSE_RESP) + " ERR\n";
        write_all(fd, message.c_str(), message.size());
        return;
    }
    
    string message;
    switch(close(uid, password, eid)){
        case CloseResult::SUCCESS:
            message = op_to_str(OP_CLOSE_RESP) + " OK\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::WRONG_USER_PASS:
            message = op_to_str(OP_CLOSE_RESP) + " NOK\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::NOT_LOGGED_IN:
            message = op_to_str(OP_CLOSE_RESP) + " NLG\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::EVENT_DOES_NOT_EXIST:
            message = op_to_str(OP_CLOSE_RESP) + " NOE\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::NOT_CREATED_BY_USER:
            message = op_to_str(OP_CLOSE_RESP) + " EOW\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::SOLD_OUT:
            message = op_to_str(OP_CLOSE_RESP) + " SLD\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::ALREADY_PASSED:
            message = op_to_str(OP_CLOSE_RESP) + " PST\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::ALREADY_CLOSED:
            message = op_to_str(OP_CLOSE_RESP) + " CLO\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case CloseResult::IO_ERROR:
            write_all(fd, "NOK\n", 4);
            return;
    }  
}

string event_list_to_string(vector<Event_list> &events){
    ostringstream oss;
    bool first = true;

    for (auto &e : events) {
        if (!first) oss << " ";
        oss << e.eid << " " << e.name << " " << e.state << " " << e.datetime.toString(false); 
        first = false;
    }

    return oss.str();
}

void handle_list(int fd, string request_so_far){
    if(!parse_list_request(request_so_far.c_str())){
        string message = op_to_str(OP_LIST_RESP) + " ERR\n";
        write_all(fd, message.c_str(), message.size());
        return;
    }

    vector<Event_list> events;
    string message;
    switch(list(events)){
        case ListResult::SUCCESS:
            message = op_to_str(OP_LIST_RESP) + " OK " + event_list_to_string(events) + "\n";
            write_all(fd, message.c_str(), message.size());
            return;
        case ListResult::NO_EVENT_CREATED:
            message = op_to_str(OP_LIST_RESP) + " NOK\n";
            write_all(fd, message.c_str(), message.size());
            return;
         case ListResult::IO_ERROR:
            write_all(fd, "NOK\n", 4);
            return;
    }   
}

void handle_show(int fd, string request_so_far){
    (void)fd;
    (void)request_so_far;
    cout << "SHOW" << endl;
}

void handle_reserve(int fd, string request_so_far){
    (void)fd;
    (void)request_so_far;
    cout << "RESERVE" << endl;
}

void handle_changePass(int fd, string request_so_far){
    (void)fd;
    (void)request_so_far;
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

void process_TCP_request(int fd, OP_CODE code, string request_so_far){
    if(code == OP_CREATE) 
        handle_create(fd, request_so_far);
    else if(code == OP_CLOSE) 
        handle_close(fd, request_so_far);
    else if(code == OP_LIST) 
        handle_list(fd, request_so_far);
    else if(code == OP_SHOW) 
        handle_show(fd, request_so_far);
    else if(code == OP_RESERVE) 
        handle_reserve(fd, request_so_far);
    else if(code == OP_CHANGE_PASS) 
        handle_changePass(fd, request_so_far);
    else cerr << "[HANDLER] Invalid OP_CODE passed through TCP";    
}