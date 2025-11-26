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

#endif