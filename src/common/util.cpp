#include <iostream>
#include <string.h>

#include "constants.hpp"

bool is_valid_userid(char *s){
    if (strlen(s) != UID_SIZE - 1) return false;
    for (int i = 0; i < UID_SIZE - 1; i++)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

bool is_valid_password(char *s){
    if (strlen(s) != PASS_SIZE - 1) return false;
    for (int i = 0; i < PASS_SIZE - 1; i++)
        if (!isalnum((unsigned char)s[i])) return false;
    return true;
}