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

#endif