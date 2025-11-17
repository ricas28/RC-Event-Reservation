#ifndef __PARSER__HPP__
#define __PARSER__HPP__

#include <string>

#include "../common/Date.hpp"
#include "commands.hpp"

using namespace std;

/**
 * Parses a command, separating the actual command from the arguments.
 * 
 * @param line String with the whole command.
 * @param args Pointer to a string that will contain the arguments.
 * 
 * @returns Command being executed.
 */
enum Command parse_command(char *line, char **args);

/**
 * Parses the command line arguments.
 * 
 * @param port Addres of string representing the server's port.
 * (If this parameter is not present it will be equal to DEFAULT_PORT: 58009)
 * @param ip Addres of a string representing the server's ip.
 * (If this parameter is not present, we assume the server is hosted locally)
 * @param argv Command line arguments.
 * @param argc Number of arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_args(string &port, string &ip, char** argv, int argc);

/* -------------------------------------------------------------------------- */

/**
 * Parses a login command.
 * 
 * @param args String with arguments.
 * @param uid Pointer to store user id.
 * @param pass Pointer to password.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_login(char *args, int *uid, string *pass);

/**
 * Parses a changePass command.
 * 
 * @param args String with arguments.
 * @param old_pass Pointer to old password.
 * @param new_pass Pointer to new password.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_change_pass(char *args, string *old_pass, string *new_pass);

/**
 * Parses a unregister command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_unregister(char *args);

/**
 * Parses a logout command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_logout(char *args);

/**
 * Parses an exit command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_exit(char *args);

/**
 * Parses a myevents command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myevents(char *args);

/**
 * Parses a list command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_list(char *args);

/**
 * Parses a myreservations command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myreservations(char *args);

/**
 * Parses a create command.
 * 
 * @param args String with arguments.
 * @param name Pointer to a string that will contain the event's name.
 * @param event_fname Pointer to a string that will contain the name of the 
 *  file with the event's details.
 * @param event_date Pointer to a Date object that will contain the event's date.
 * @param num_attendees Pointer to an integer that will contain the number of
 *  people who can attend the event.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_create(char *args, string *name, string *event_fname, 
                                        Date *event_date, int *num_attendees);

/**
 * Parses a close command.
 * 
 * @param args String with arguments.
 * @param eid Pointer to an int that will contain the event id.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_close(char *args, int *eid);

/**
 * Parses a show command.
 * 
 * @param args String with arguments.
 * @param eid Pointer to an int that will contain the event id.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_show(char *args, int *eid);

/**
 * Parses a reserve command.
 * 
 * @param args String with arguments.
 * @param eid Pointer to an integer that will contain the event id.
 * @param seats Pointer to an integer that will contain the number of seats
 *  to be reserved.
 * 
 * @return true if parse is successful, false otherwise.
 */
bool parse_reserve(char *args, int *eid, int *seats);

#endif