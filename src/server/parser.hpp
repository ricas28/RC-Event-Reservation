#ifndef __S_PARSER__HPP__
#define __S_PARSER__HPP__

#include <iostream>
#include <string>

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

#endif