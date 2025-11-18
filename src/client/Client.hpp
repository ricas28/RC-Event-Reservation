#ifndef __CLIENT__HPP__
#define __CLIENT__HPP__

#include <iostream>
using namespace std;

typedef struct ClLArgs{
    int uid = -1, udp_socket = -1, tcp_socket = -1;
    string pass = "";
    bool logged_in = false;
}CLArgs;

#endif