#ifndef __C_PARSER__HPP__
#define __C_PARSER__HPP__

#include <string>
#include <vector>

#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"
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
Command parse_command(char *line, char **args);

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
 * Parses a response to the 'login' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_login_response(char *response, string &status);

/**
 * Parses a 'changePass' command.
 * 
 * @param args String with arguments.
 * @param old_pass Pointer to old password.
 * @param new_pass Pointer to new password.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_changePass(char *args, string *old_pass, string *new_pass);

/**
 * Parses a response to the 'changePass' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_changePass_response(char *response, string &status);

/**
 * Parses a 'unregister' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_unregister(char *args);

/**
 * Parses a response to the 'unregister' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_unregister_response(char *response, string &status);

/**
 * Parses a 'logout' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_logout(char *args);

/**
 * Parses a response to the 'logout' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_logout_response(char *response, string &status);

/**
 * Parses an 'exit' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_exit(char *args);

/**
 * Parses a 'myevents' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myevents(char *args);

/**
 * Parses a response to the 'myevents' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * @param events_list vector with events created by the user if command is successful.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myevents_response(char *response, string &status, 
                            vector<pair<string, int>> &events_list);

/**
 * Parses a 'list' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_list(char *args);

/**
 * Parses a response to the 'list' command.
 * 
 * @param response Response form the server.
 * @param status String that will contain the value of the status field.
 * @param events_list vector with all events ever created if command is successful.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_list_response(const char *response, string &status, vector<Event_list> &events_list);

/**
 * Parses a 'myreservations' command.
 * 
 * @param args String with (hopefully) no arguments.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myreservations(char *args);

/**
 * Parses a response to the 'myreservations' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * @param events_list vector with reservations made by the user if command is successful.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_myreservations_response(char *response, string &status, vector<Reservation> &reservations_list);

/**
 * Parses a 'create' command.
 * 
 * @param args String with arguments.
 * @param name Pointer to a string that will contain the event's name.
 * @param event_fname Pointer to a string that will contain the name of the 
 *  file with the event's details.
 * @param Fsize Pointer to size of the file that was inputed.
 * @param Fdata Pointer to a pointer to file content.
 * @param event_date Pointer to a Date object that will contain the event's date.
 * @param num_attendees Pointer to an integer that will contain the number of
 *  people who can attend the event.
 * 
 * @returns true if parse is successful, false otherwise.
 * 
 * Note: After use Fdata needs to be freed using free().
 */
bool parse_create(char *args, string *name, string *event_fname, size_t *Fsize,
                        char **Fdata, DateTime *event_date, int *num_attendees);


/**
 * Parses a response to the 'create' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * @param eid String that will contain event id from the created event if
 *      command was successful .
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_create_response(const char *response, string &status, string &eid);

/**
 * Parses a 'close' command.
 * 
 * @param args String with arguments.
 * @param eid Pointer to a string that will contain the event id.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_close(char *args, string &eid);

/**
 * Parses a response to the 'close' command.
 * 
 * @param response Response from the server.
 * @param status String that will contain the value of the status field.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_close_response(const char *response, string &status);

/**
 * Parses a 'show' command.
 * 
 * @param args String with arguments.
 * @param eid String that will contain the event id.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_show(char *args, string &eid);

/**
 * @brief Parses a response to the 'show' command.
 * 
 * This functions works differently from the others. Instead of 
 * only checking a response's parameters, it reads and then validates
 * due to the structure of the 'show' command. This function should be 
 * called only if status on the response is 'OK'. It has undefined behaviour
 * for other cases.
 * 
 * @param fd TCP socket.
 * @param event Struct with event information.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_show_response(int fd, Event_Info &event);

/**
 * Parses a 'reserve' command.
 * 
 * @param args String with arguments.
 * @param eid String that will contain the event id.
 * @param seats Integer that will contain the number of seats to be reserved.
 * 
 * @return true if parse is successful, false otherwise.
 */
bool parse_reserve(char *args, string &eid, int &seats);

/**
 * Parses a response to the 'reserve' command.
 * 
  *@param response Response from the server.
 * @param status String that will contain the value of the status field.
 * @param n_seats Integer that contains remaining seats on the event if
 *  command is rejected.
 * 
 * @returns true if parse is successful, false otherwise.
 */
bool parse_reserve_response(char *response, string &status, int &n_seats);

#endif