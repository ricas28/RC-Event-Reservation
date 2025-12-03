#ifndef __API__HPP__
#define __API__HPP__

#include "Client.hpp"

/**
 * Executes the 'login' command.
 * 
 * @param client Client.
 * @param uid User id that user typed in.
 * @param pass Password that user typed in.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_login(CLArgs client, int uid, string pass);

/**
 * Executes the 'logout' command.
 * 
 * @param client Client.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_logout(CLArgs client);

/**
 * Executes the 'unregister' command.
 * 
 * @param client Client.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_unregister(CLArgs client);

/**
 * Executes the 'myevents' command.
 * 
 * @param client Client.
 * 
 * @return 0 for successful execution, -1 on failure.  
*/ 
int er_myevents(ClLArgs client);

/**
 * Executes the 'create' command.
 * 
 * @param client Client
 * @param name Name of the event.
 * @param event_fname Name of the file with event content.
 * @param Fsize Size of the file in bytes.
 * @param Fdata Contento of the file.
 * @param event_date Date and time of the event.
 * @param num_attendees Number of people that will attend the event.
 * 
 * @return 0 for successful execution, -1 on failure.
 */
int er_create(CLArgs client, string name, string event_fname, size_t Fsize,
                        char *Fdata, DateTime event_date, int num_attendees);

#endif