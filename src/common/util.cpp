#include <iostream>
#include <string.h>
#include <filesystem>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>

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
    if (size == 0 || size > EVENT_NAME_SIZE - 1) return false;
    for(size_t i = 0; i < size - 1; i++)
        if (!isalnum(s[i])) return false;
    return true;
}

bool is_valid_file_name(char *s){
    size_t size = strlen(s);
    if (size == 0 || size > FILE_NAME_SIZE - 1) return false;
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
    return !date_time.invalidDateTime() && !date_time.isPast();
}

bool is_valid_num_attendees(int num_attendees){
    return num_attendees >= 10 && num_attendees <= MAX_SEATS;
}

bool is_valid_eid(char *s){
    if (strlen(s) != EID_SIZE - 1) return false;
    for(int i = 0; i < EID_SIZE - 1; i++)
        if (!isdigit(s[i])) return false;
    return true;
}

/** 
 * Checks if a given string is an integer (has option for allowing 0 or not)
 * 
 * @param s String.
 * @param allow_zero Bool that allows 0 to be considered or not.
 * @param out_value Pointer to converted integer.
 * 
 * @returns true if it's a valid integer, false otherwise.
 */
bool is_integer_internal(const char* s, bool allow_zero, int* out_value) {
    if (!s || *s == '\0' || s[0] == '-' || s[0] == '+') 
        return false;

    int value = 0;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        if (!isdigit(s[i])) return false;
        int digit = s[i] - '0';
        if (value > (std::numeric_limits<int>::max() - digit) / 10)
            return false; // overflow
        value = value * 10 + digit;
    }

    if (!allow_zero && value == 0) return false;

    if (out_value) *out_value = value;
    return true;
}

bool is_positive_integer(const char* s, int* out_value = nullptr) {
    return is_integer_internal(s, false, out_value);
}

bool is_nonnegative_integer(const char* s, int* out_value = nullptr) {
    return is_integer_internal(s, true, out_value);
}

bool is_valid_seats(char *s, int *seats = nullptr){
    int value;
    if (!is_positive_integer(s, &value)) return false;
    if(value > MAX_SEATS) return false;

    if(seats) *seats = value;
    return true;
}

bool is_valid_event_state(int state){
    return state == EVENT_ACCEPTING || state == EVENT_IN_PAST ||
            state == EVENT_CLOSED || state == EVENT_SOLD_OUT;
}

void print_event_state(int state){
    switch(state){
        case EVENT_ACCEPTING:
            cout << "Event still accepting" << endl;
            break;
        case EVENT_IN_PAST:
            cout << "Event already occured" << endl;
            break;
        case EVENT_SOLD_OUT:
            cout << "Event is sold out" << endl;
            break;
        case EVENT_CLOSED:
            cout << "Event closed by the user" << endl;
            break;
        case EVENT_CORRUPTED:
            cout << "Event information is corrupted" << endl;
            break;
        default:
            cerr << "Invalid state" << endl;
    }
}

void extract_ip_port_in(const struct sockaddr_in *addr,
                        char *ip,
                        size_t ip_size,
                        char *port,
                        size_t port_size){
    // IP
    inet_ntop(AF_INET, &(addr->sin_addr), ip, (socklen_t)ip_size);

    // PORT
    snprintf(port, port_size, "%u", ntohs(addr->sin_port));
}

bool safe_create_dir(const std::string &path){
    try {
        std::error_code ec;
        if (!fs::create_directories(path, ec) && ec) {
            cerr << "[ERROR] Failed to create directory: " << path
                 << " — " << ec.message() << endl;
            return false;
        }
        return true;
    } catch (std::exception &e) {
        cerr << "[EXCEPTION] Creating directory: " << path
             << " — " << e.what() << endl;
        return false;
    }
}

string format_eid(int eid){
    ostringstream oss;
    oss << setw(3) << setfill('0') << eid;
    return oss.str();
}