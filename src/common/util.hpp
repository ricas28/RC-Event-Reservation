#ifndef __UTIL__HPP__
#define __UTIL__HPP__

/**
 * Chekcs if the content of a string represents a valid user id.
 * 
 * @param s String.
 * 
 * @returns true if has a valid user id, false otherwise.
 */
bool is_valid_userid(char *s);

/**
 * Chekcs if the content of a string represents a valid password.
 * 
 * @param s String.
 * 
 * @returns true if has a valid password, false otherwise.
 */
bool is_valid_password(char *s);

#endif
