#ifndef __S_PARSER__HPP__
#define __S_PARSER__HPP__

#include <iostream>
#include <string>

#include "../common/protocol.hpp"

using namespace std;

/**
 * Parses the command line arguments for running the server.
 * 
 * @param port String that will contain the port where the server is running.
 * @param verbose Bool that will contain if the server is running on verbose
 *  mode or not.
 * @param argv Command line arguments received.
 * @param argc Number of command line arguments received.
 * 
 * @return true for successful parse, false otherwise.
 */
bool parse_args(string &port, bool &verbose, char** argv, int argc);

/**
 * Receives a string with the code for a request and returns 
 * it's OP_CODE form.
 * This function validates if the given command is a command
 * that is handled with TCP or not.
 * 
 * @param command String with code for request.
 * 
 * @return OP_CODE form of code or ERR for invalid code.
 */
OP_CODE get_tcp_command(const char *command);

/**
 * Receives a string with the code for a request and returns 
 * it's OP_CODE form.
 * This function validates if the given command is a command
 * that is handled with UDP or not.
 * 
 * @param command String with code for request.
 * 
 * @return OP_CODE form of code or ERR for invalid code.
 */
OP_CODE get_udp_command(const char *command);

#endif