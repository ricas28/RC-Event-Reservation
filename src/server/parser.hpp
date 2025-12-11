#ifndef __S__PARSER__HPP__
#define __S__PARSER__HPP__

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


/* ------------- COMMAND PARSING -------------- */

/**
 * Parses a request to the 'login' command.
 * 
 * @param request Request that was received.
 * @param uid String that will contain the UID sent on the request.
 * @param password String that will contain the password sent on the request.
 */
bool parse_login_request(const char *request, string &uid, string &password);

/**
 * Parses a request to the 'logout' command.
 * 
 * @param request Request that was received.
 * @param uid String that will contain the UID sent on the request.
 * @param password String that will contain the password sent on the request.
 */
bool parse_logout_request(const char *request, string &uid, string &password);

/**
 * Parses a request to the 'unregister' command.
 * 
 * @param request Request that was received.
 * @param uid String that will contain the UID sent on the request.
 * @param password String that will contain the password sent on the request.
 */
bool parse_unregister_request(const char *request, string &uid, string &password);

#endif