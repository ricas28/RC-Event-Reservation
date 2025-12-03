#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "constants.hpp"
#include "protocol.hpp"

using namespace std;

string op_to_str(OP_CODE code){
    if(code == OP_LOGIN) return "LIN";
    if(code == OP_LOGIN_RESP) return "RLI";
    if(code == OP_CHANGE_PASS) return "CPS";
    if(code == OP_CHANGE_PASS_RESP) return "RCP";
    if(code == OP_UNREGISTER) return "UNR";
    if(code == OP_UNREGISTER_RESP) return "RUR";
    if(code == OP_LOGOUT) return "LOU";
    if(code == OP_LOGOUT_RESP) return "RLO";
    if(code == OP_CREATE) return "CRE";
    if(code == OP_CREATE_RESP) return "RCE";
    if(code == OP_CLOSE) return "CLS";
    if(code == OP_CLOSE_RESP) return "RCL";
    if(code == OP_MYEVENTS) return "LME";
    if(code == OP_MYEVENTS_RESP) return "RME";
    if(code == OP_LIST) return "LST";
    if(code == OP_LIST_RESP) return "RLS";
    if(code == OP_RESERVE) return "RID";
    if(code == OP_RESERVE_RESP) return "RRI";
    if(code == OP_MYRESERVATIONS) return "LMR";
    if(code == OP_MYRESERVATIONS_RESP) return "RMR";
    if(code == ERR) return "ERR";
    return "INV";
}

OP_CODE str_to_op(const char *str){
    if(strcmp(str, "LIN") == 0) return OP_LOGIN;
    if(strcmp(str, "RLI") == 0) return OP_LOGIN_RESP;
    if(strcmp(str, "CPS") == 0) return OP_CHANGE_PASS;
    if(strcmp(str, "RCP") == 0) return OP_CHANGE_PASS_RESP;
    if(strcmp(str, "UNR") == 0) return OP_UNREGISTER;
    if(strcmp(str, "RUR") == 0) return OP_UNREGISTER_RESP;
    if(strcmp(str, "LOU") == 0) return OP_LOGOUT;
    if(strcmp(str, "RLO") == 0) return OP_LOGOUT_RESP;
    if(strcmp(str, "CRE") == 0) return OP_CREATE;
    if(strcmp(str, "RCE") == 0) return OP_CREATE_RESP;
    if(strcmp(str, "CLS") == 0) return OP_CLOSE;
    if(strcmp(str, "RCL") == 0) return OP_CLOSE_RESP;
    if(strcmp(str, "LME") == 0) return OP_MYEVENTS;
    if(strcmp(str, "RME") == 0) return OP_MYEVENTS_RESP;
    if(strcmp(str, "LST") == 0) return OP_LIST;
    if(strcmp(str, "RLS") == 0) return OP_LIST_RESP;
    if(strcmp(str, "RID") == 0) return OP_RESERVE;
    if(strcmp(str, "RRI") == 0) return OP_RESERVE_RESP;
    if(strcmp(str, "LMR") == 0) return OP_MYRESERVATIONS;
    if(strcmp(str, "RMR") == 0) return OP_MYRESERVATIONS_RESP;
    if(strcmp(str, "ERR") == 0) return ERR;
    return OP_INVALID;
}

int send_udp_message(int socket, const char *message, const struct addrinfo *addr){
    ssize_t n = sendto(socket, message, strlen(message), 0, addr->ai_addr, addr->ai_addrlen);
    if (n == -1) {
        perror("Failure to send message with UDP");
        return -1;
    }
    return 0;
}

char *receive_udp_message(int socket, struct addrinfo *addr) {
    char *buffer = (char *)malloc(UDP_BUFFER_SIZE);
    if (!buffer) return NULL;
    
    socklen_t addrlen = addr->ai_addrlen;
    ssize_t n = recvfrom(socket, buffer, UDP_BUFFER_SIZE, 0, addr->ai_addr, &addrlen);

    if (n == -1) {
        perror("Failure to receive message with UDP");
        free(buffer);
        return NULL;
    }
    
    buffer[n] = '\0';
    return buffer;
}

void close_socket_connection(int socket, struct addrinfo *addr){
    free(addr);
    close(socket);
}
