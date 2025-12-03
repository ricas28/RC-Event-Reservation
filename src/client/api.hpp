#ifndef __API__HPP__
#define __API__HPP__

#include "Client.hpp"

/**
 * Executes the 'login' command.
 * 
 * @param client Pointer to a client.
 * @param uid User id that user typed in.
 * @param pass Password that user typed in.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_login(CLArgs *client, int uid, string pass);

/**
 * Executes the 'logout' command.
 * 
 * @param client Pointer to a client.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_logout(CLArgs *client);

/**
 * Executes the 'unregister' command.
 * 
 * @param client Pointer to a client.
 * 
 * @returns 0 for successful execetion, -1 on failure.
 */
int er_unregister(CLArgs *client);

/**
 * Executes the 'myevents' command.
 * 
 * @param client Client.
 * 
 * @return 0 for successful execution, -1 on failure.  
*/ 
int er_myevents(ClLArgs client);

/**
 * Executes the 'myreservations' command.
 * 
 * @param client Client.
 * 
 * @return 0 for successful execution, -1 on failure.  
*/ 
int er_myreservations(ClLArgs client);

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

/**
 * Executes the 'close' command.
 * 
 * @param client Client.
 * @param eid Id of the event being closed.
 * 
 * @returns 0 for successful execution, -1 on failure.
 */
int er_close(CLArgs client, string eid);

/**
 * Executes the 'list' command.
 * 
 * @param client Client.
 * 
 * @returns 0 for successful execution, -1 on failure.
 */
int er_list(ClLArgs client);

/**
 * Executes the 'show' command.
 * 
 * @param client Client.
 * @param eid String with event id of the event being shown.
 * 
 * @returns 0 for successful execution, -1 on failure.
 */
int er_show(CLArgs client, string &eid);

/**
 * Executes the 'reserve' command.
 * 
 * @param client Client.
 * @param eid String with event id of the event being reserved.
 * @param people Number of seats being reserved.
 * 
 * @returns 0 for successful execution, -1 on failure.
 */
int er_reserve(CLArgs client, string &eid, int people);

#endif