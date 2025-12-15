#ifndef __OPERATIONS__HPP__
#define __OPERATIONS__HPP__

#include <iostream>
#include <vector>

#include "resultCode.hpp"

using namespace std;

/** 
 * Initalizes the Database
 *
 * @returns 0 on succes, -1 on failure.  
*/
int db_init();

/**
 * Executes a 'login' command
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * 
 * @return LoginResult enum.
 */
LoginResult login(string &uid, string &password);

/**
 * Executes a 'logout' command
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * 
 * @return LogoutResult enum.
 */
LogoutResult logout(string &uid, string &password);

/**
 * Executes a 'unregister' command
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * 
 * @return UnregisterResult enum.
 */
UnregisterResult unregister(string &uid, string &password);

/**
 * Executes a 'myevents' command
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * @param events Vector that will contain the events the user created and
 *  their state if they have created any events.
 * 
 * @return MyEventsResult enum.
 */
MyEventsResult myevents(string &uid, string &password, vector<pair<string, int>> &events);

/**
 * Executes a 'myreservations' command.
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * @param reservations Vector that will contain the 50 most recent reservations
 *  made by the user.
 * 
 * @return MyReservatiionsResult enum.
 */
MyReservationsResult myreservations(string &uid, string &password, vector<Reservation> &reservations);

/**
 * Executes a 'create' command.
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * @param event Struct with info for creating event.
 * @param eid EID of the new created event.
 * 
 * @return CreateResult enum.
 */
CreateResult create(string &uid, string &password, Event_creation_Info &event, string &eid);

/**
 * Executes a 'close' command.
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * @param eid EID of the new created event.
 * 
 * @return CloseResult enum.
 */
CloseResult close(string &uid, string &password, string &eid);

/**
 * Executes a 'list' command.
 * 
 * @param events Vector that will contain every event on the server.
 * 
 * @return ListResult enum.
 */
ListResult list(vector<Event_list> &events);

/**
 * Executes a 'reserve' command.
 * 
 * @param uid UID received on the request.
 * @param password Password received on the request.
 * @param eid EID of the event being reserved.
 * @param people Number of seats being reserved.
 * @param remaining_seats Integer that will have number of remaining seats
 *  on the event if reservation is rejected.
 * 
 * @return ReserveResult enum.
 */
ReserveResult reserve(string &uid, string &password, string &eid, int &people, int &remaining_seats);

/**
 * Executes a 'changePass' command.
 * 
 * @param uid UID received on the request.
 * @param old_password Old password received on the request.
 * @param new_password New password received on the request.
 *
 * @return ChangePassResult enum.
 */
ChangePassResult changePass(string &uid, string &old_password, string &new_password);

#endif