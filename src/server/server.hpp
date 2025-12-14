#ifndef __SERVER__HPP__
#define __SERVER__HPP__

#include <iostream>
 
#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"

using namespace std;

struct StartFileData{
    int uid;
    string event_name;
    string desc_fname;
    int event_attend;
    DateTime start_date_time;
};

/**
 * Initializes the parameters for the server.
 * 
 * @param port Where the server is running.
 * @param tcp_socket Int that will contain a TCP socket.
 * @param udp_socket Int that will contain a UDP socket.
 * 
 * @return 0 on success, -1 on failure.
 */
int server_init(string port, int &tcp_socket, int &udp_socket);

/** Closes file descriptors for TCP and UDP messages.*/
void destroy_server(int tcp_socket, int udp_socket);

/**
 * Handles a TCP request.
 * This function already reads the OP_CODE and the UID, if applicable,
 * from the TCP stream.
 * 
 * @param fd TCP socket.
 * @param client_addr Client address.
 * @param verbose Bool to activate server verbose mode.
 * 
 * @returns 0 on success, -1 on failure.
 */
int handle_tcp_request(int fd, struct sockaddr_in client_addr, bool verbose);

/**
 * Handles a UDP request.
 * 
 * @param fd UDP socket.
 * @param verbose (Optional) Bool to activate server verbose mode.
 * @returns 0 on success, -1 on failure.
 */
int handle_udp_request(int udp_fd, bool verbose= false);

/**
 * Extracts information of a START file into a StartFileData.
 * 
 * @param filepath Filepath of the START file.
 * 
 * @returns StartFileData on success, struct with invalid values on failure.
 */
StartFileData extract_start_file_data(const string &filepath);

/**
 * Extracts information of a reservation file
 * 
 * @param filepath Filepath of the reservation file.
 * 
 * @returns Reservation struct on success, struct in invalid values on failure.
 */
Reservation extract_reservation_file_data(const string &filepath);

#endif