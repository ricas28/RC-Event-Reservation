#include <iostream>
#include <string.h>
#include <filesystem>

#include "constants.hpp"
#include "DateTime.hpp"
#include "protocol.hpp"

// Simplification
namespace fs = std::filesystem;

bool is_valid_userid(char *s){
    if (strlen(s) != UID_SIZE - 1) return false;
    for(int i = 0; i < UID_SIZE - 1; i++)
        if (!isdigit(s[i])) return false;
    return true;
}

bool is_valid_password(char *s){
    if (strlen(s) != PASS_SIZE - 1) return false;
    for(int i = 0; i < PASS_SIZE - 1; i++)
        if (!isalnum(s[i])) return false;
    return true;
}

bool is_valid_event_name(char *s){
    size_t size = strlen(s);
    if (size > EVENT_NAME_SIZE - 1) return false;
    for(size_t i = 0; i < size - 1; i++)
        if (!isalnum(s[i])) return false;
    return true;
}

bool is_valid_file_name(char *s){
    size_t size = strlen(s);
    if (size > FILE_NAME_SIZE - 1) return false;
    for(size_t i = 0; i < size - 1; i++)
        if (!isalnum(s[i]) && s[i] != '.' && s[i] != '-' && s[i] != '_')
            return false;
    return true;
}

bool file_exists(char *file_name){
    return fs::exists(file_name) && fs::is_regular_file(file_name);
}

bool is_valid_date_time(int day, int month, int year, int hour, int minute){
    DateTime date_time(day, month, year, hour, minute);
    return !date_time.invalidDateTime() and !date_time.isPast();
}

bool is_valid_num_attendees(int num_attendees){
    return num_attendees >= 10 and num_attendees <= 999;
}

bool is_valid_eid(char *s){
    if (strlen(s) != EID_SIZE - 1) return false;
    for(int i = 0; i < EID_SIZE - 1; i++)
        if (!isdigit(s[i])) return false;
    return true;
}

bool is_positive_integer(char *s) {
    // Can't start with '-', '+' or '0'.
    if (s[0] == '-' || s[0] == '+' || s[0] == 0) return false;

    size_t size = strlen(s);
    for (size_t i = 0; i < size-1; i++) 
        // Each char must be a digit.
        if (!isdigit(s[i]))
            return false;
    return true;
}

bool is_valid_event_state(int state){
    return state == EVENT_ACCEPTING || state == EVENT_IN_PAST ||
            state == EVENT_CLOSED || state == EVENT_SOLD_OUT;
}