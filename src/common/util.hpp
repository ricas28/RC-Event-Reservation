#ifndef __UTIL__HPP__
#define __UTIL__HPP__

/**
 * Checks if the content of a string represents a valid user id.
 * 
 * @param s String.
 * 
 * @returns true if has a valid user id, false otherwise.
 */
bool is_valid_userid(char *s);

/**
 * Checks if the content of a string represents a valid password.
 * 
 * @param s String.
 * 
 * @returns true if has a valid password, false otherwise.
 */
bool is_valid_password(char *s);

/**
 * Checks if the content of a string represents a valid event name.
 * 
 * @param s String.
 * 
 * @returns true if has a valid event name, false otherwise.
 */
bool is_valid_event_name(char *s);

/**
 * Checks if the content of a string represents a valid file name.
 * 
 * @param s String.
 * 
 * @returns true if has a valid file name, false otherwise.
 */
bool is_valid_file_name(char *s);

/**
 * Checks if a file with the given name existe on the current directory.
 * 
 * @param file_name String reprsenting the name of a file.
 * 
 * @returns true if file exists on the current directory, false otherwise.
 */
bool file_exists(char *file_name);

/**
 * Checks if a date and time with the given attributes is valid.
 * (Valid date and time and not in the past).
 * 
 * @param day Day of the date.
 * @param month Month of the date.
 * @param year Year of the date.
 * @param hour Hour of the time.
 * @param minute Minute of the time
 * 
 * @returns true if it's a valid date, false otherwise.
 */
bool is_valid_date_time(int day, int month, int year, int hour, int minute);

/**
 * Checks if the given number is a valid number of attendees (10-999).
 * 
 * @param num_attendees Number of attendees.
 * 
 * @returns true if it's a valid number, false otherwise.
 */
bool is_valid_num_attendees(int num_attendees);

/**
 * Checks if the content of a string represents a valid event id.
 * 
 * @param s String.
 * 
 * @returns true if has a valid event id, false otherwise.
 */
bool is_valid_eid(char *s);

/**
 * Checks if the content of a string has a valid number of seats (1-999).
 * 
 * @param s String.
 * @param seats (Optional) Pointer to int that will contain converted int 
 * if it's valid 
 * 
 * @returns true if it's valid, false otherwise.
 */
bool is_valid_seats(char *s, int *seats = nullptr);

/** 
 * Checks if a given string is a positiv integer.
 * 
 * @param s String.
 * @param out_value (Optional) Stores the value if it's a positive integer.
 * 
 * @returns true if it's a positive integer, false otherwise.
 */
bool is_positive_integer(const char* s, int* out_value = nullptr);

/**
 * Does the same as is_positive_integer but includes 0 value.
 * 
 * @param s String.
 * @param out_value (Optional) Stores the value if it's a positive integer.
 * 
 * @returns true if it's a nonnegative integer, false otherwise.
 */
bool is_nonnegative_integer(const char* s, int* out_value = nullptr);

/**
 * Cheks if the given event state is valid
 * 
 * @param state event state
 * 
 * @return true if state is vaid, false otherwise.
 */
bool is_valid_event_state(int state);

/**
 * Prints a message according to the event state.
 */
void print_event_state(int state);

/**
 * Extracts the ip and port from a struct sockaddr_storage struct.
 * 
 * @param addr sockaddr_storage struct with address info.
 * @param addrlen Size of address.
 * @param ip String that will contain the ip of the address.
 * @param ip_size Size of the ip string.
 * @param port String that will contain the port of the address
 * @param port_size Size of the port string.
 */
void extract_ip_port_storage(const struct sockaddr_storage *addr,
                             socklen_t addrlen,
                             char *ip,
                             size_t ip_size,
                             char *port,
                             size_t port_size);

/**
 * Extracts the ip and port from a struct sockaddr_in struct.
 * 
 * @param addr sockaddr_in struct with address info.
 * @param ip String that will contain the ip of the address.
 * @param ip_size Size of the string.
 * @param port String that will contain the port of the address
 * @param port_size Size of the port string.
 */
void extract_ip_port_in(const struct sockaddr_in *addr,
                        char *ip,
                        size_t ip_size,
                        char *port,
                        size_t port_size);

#endif
