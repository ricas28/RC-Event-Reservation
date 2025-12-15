#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>           
#include <vector>

#include "server.hpp"
#include "command_handler.hpp"
#include "parser.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "../common/util.hpp"
#include "../common/io.hpp"
#include "../common/DateTime.hpp"

using namespace std;

#define MAX_CONNECTIONS 5

/**
 * Sets up the TCP server socket.
 *  
 * @param port Port where the server is running.
 * 
 * @returns Fd for socket on success, -1 on failure.
 */
int set_server_tcp_socket(string port){
    struct addrinfo hints, *res;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror ("Failure to open TCP socket");
        return -1;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if ((errcode) != 0) {
        perror("Failure to do DNS lookup for TCP socket");
        close(fd);
        return -1;
    }
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed on TCP");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }
    int n = bind(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        perror("Failure to bind TCP socket to adress");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }
    if (listen(fd, MAX_CONNECTIONS) == -1) {
        perror("Failure to execute listen function");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }

    freeaddrinfo(res);
    return fd;
}

/**
 * Sets up the UDP server socket.
 *  
 * @param port Port where the server is running.
 * 
 * @returns Fd for socket on success, -1 on failure.
 */
int set_server_udp_socket(string port){
    struct addrinfo hints, *res;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    int errcode = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if (errcode != 0) {
        close(fd);
        return -1;
    }
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed on UDP");
        freeaddrinfo(res);
        close(fd);
        return -1;
    }
    int n = bind(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        freeaddrinfo(res);
        close(fd);
        return -1;
    }

    freeaddrinfo(res);
    return fd;
}

int server_init(string port, int &tcp_socket, int &udp_socket){
    if((tcp_socket = set_server_tcp_socket(port)) == -1)
        return -1;

    if((udp_socket = set_server_udp_socket(port)) == -1){
        close(tcp_socket);
        return -1;
    }

    return 0;
}

void print_verbose(OP_CODE code, const char *request, struct sockaddr_in client_addr){
    char ip[INET_ADDRSTRLEN], port[16];

    // Extract ip and port from address.
    extract_ip_port_in(&client_addr, ip, sizeof(ip), port, sizeof(port));

    string code_str = op_to_str(code);
    
    if(has_uid(code)){
        char extracted_uid[BUF_TEMP];
        int n = sscanf(request, "%*s %63s", extracted_uid);

        if(n != 1){
            cout << "[VERBOSE] ERROR: missing UID " << " | REQUEST TYPE: " << code_str;
            cout << " | FROM: " << ip << ":" << port << endl;
            return;
        }  
        cout << "[VERBOSE] UID: " << extracted_uid << " | REQUEST TYPE: " << code_str;
        cout << " | FROM: " << ip << ":" << port << endl;
    }
    else{
        cout << "[VERBOSE] REQUEST TYPE: " << op_to_str(code);
        cout << " | FROM: " << ip << ":" << port << endl;
    }
}   

void destroy_server(int tcp_socket, int udp_socket){
    close(tcp_socket);
    close(udp_socket);
}

int handle_tcp_request(int fd, struct sockaddr_in client_addr, bool verbose){
    bool end_line = false;
    string request_code = tcp_read_word(fd, &end_line);
    if(request_code == "")
        return -1;

    OP_CODE code = get_tcp_command(request_code.c_str());
    if(code == ERR){
        if(write_all(fd, "ERR\n", 4) == -1)
            cerr << "Failure to write 'ERR' message to client" << endl;
        return -1;
    }
    string request_so_far = end_line ? request_code + "\n" : request_code + " ";
    string uid;
    // Read UID if necessary wether verbose is active or not.
    if(has_uid(code)){
        // Force uid with invalid value for bad message.
        uid = end_line ? "" : tcp_read_word(fd);
        request_so_far += uid + " ";
    }
    if(verbose){
        string message = request_code + " " + uid;
        print_verbose(code, message.c_str(), client_addr);
    }
    process_TCP_request(fd, code, request_so_far);

    return 0;
}

int handle_udp_request(int fd, bool verbose){
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char request_code[BUF_TEMP];

    char *request = receive_udp_message(fd, (struct sockaddr*)&client_addr, &addrlen);
    if(!request)
        return -1;

    int n = sscanf(request, "%63s", request_code);
    if(n != 1){
        cerr << "Failure to parse request code" << endl;
        return -1;
    }
    OP_CODE code = get_udp_command(request_code);
    if(code == ERR){
        if(send_udp_message(fd, "ERR\n", (struct sockaddr*)&client_addr, addrlen) == -1)
            cerr << "Failure to write 'ERR' message to client" << endl;
        return -1;
    }
    if(verbose){
       print_verbose(code, request, client_addr);
    }
    process_UDP_request({fd, client_addr, addrlen}, code, request);

    free(request);
    return 0;
}

bool validateStartFileData(string filepath,
                            string uid, 
                            string event_name, 
                            string desc_fname, 
                            string start_date, 
                            string start_time,
                            DateTime &dt,
                            int event_attend){
    // Validate info
    if(!is_valid_userid((char *)uid.c_str())){
        cerr << "Invalid UID on file  '" << filepath << "': " << uid << endl;
        return false;;
    }
    if(!is_valid_event_name((char *)event_name.c_str())){
        cerr << "Invalid event name on file: '" << filepath << "': " << event_name << endl;
        return false;
    }
    if(!is_valid_file_name((char *)desc_fname.c_str())){
        cerr << "Invalid file name on file: '" << filepath << "': " << desc_fname << endl;
        return false;
    }
    if(!is_valid_num_attendees(event_attend)){
        cerr << "Invalid number of attendees no file '" << filepath << "': " << event_attend << endl;
        return false;
    }
    if(!DateTime::fromStrings(start_date, start_time, dt)){
        cerr << "Invalid date or time on file '" << filepath << "': " 
                                    << start_date << " " << start_time << endl;
        return false;
    }
    return true;
}

StartFileData extract_start_file_data(const std::string &filepath){
    int fd;
    if(!open_and_lock(filepath, O_RDONLY, LOCK_SH, fd))
        return { -1, "", "", -1, DateTime() };

    // Get file size.
    struct stat st;
    if(fstat(fd, &st) == -1){
        perror(("fstat: " + filepath).c_str());
        close(fd);
        return { -1, "", "", -1, DateTime() };
    }

    vector<char> buffer((size_t)st.st_size + 1, 0);
    if(read_all(fd, buffer.data(), (size_t)st.st_size) != st.st_size){
        perror(("read_all: " + filepath).c_str());
        close(fd);
        return { -1, "", "", -1, DateTime() };
    }
    close(fd); // unlock + close

    istringstream iss(buffer.data());
    string uid, event_name, desc_fname, start_date, start_time;
    int event_attend;

    if(!(iss >> uid >> event_name >> desc_fname >> event_attend >> start_date >> start_time)){
        cerr << "Invalid start file format: " << filepath << endl;
        return { -1, "", "", -1, DateTime() };
    }

    DateTime dt;
    if(!validateStartFileData(filepath, uid, event_name, desc_fname, start_date, start_time, dt, event_attend)){
        return { -1, "", "", -1, DateTime() };
    }

    return {atoi(uid.c_str()), event_name, desc_fname, event_attend, dt};
}

Reservation extract_reservation_file_data(const string &filepath){
    int fd;
    if(!open_and_lock(filepath, O_RDONLY, LOCK_SH, fd))
        return {"-1", DateTime(), -1};

    // Get file size.
    struct stat st;
    if(fstat(fd, &st) == -1){
        perror(("fstat: " + filepath).c_str());
        close(fd);
        return {"-1", DateTime(), -1};
    }

    vector<char> buffer((size_t)st.st_size + 1, 0);
    if(read_all(fd, buffer.data(), (size_t)st.st_size) != st.st_size){
        perror(("read_all: " + filepath).c_str());
        close(fd);
        return {"-1", DateTime(), -1};
    }
    close(fd); // unlock + close

    istringstream iss(buffer.data());
    string eid, date_str, time_str, num_seats_str;
    int num_seats;

    if(!(iss >> eid >> num_seats_str >> date_str >> time_str)){
        cerr << "Invalid format on file: " << filepath << endl;
        return {"-1", DateTime(), -1};
    }

    if(!is_valid_eid((char *)eid.c_str())){
        cerr << "Invalid EID on file '" << filepath  << "': "<< eid << endl;
        return {"-1", DateTime(), -1};
    }

    DateTime dt;
    if(!DateTime::fromStrings(date_str, time_str, dt)){
        cerr << "Invalid date or time on file: '" << filepath << "': " << date_str << " " << time_str << endl;
        return {"-1", DateTime(), -1};
    }

    if(!is_valid_seats((char *)num_seats_str.c_str(), &num_seats)){
        cerr << "Invalid number of reserved seats on file '" << filepath << "': " << num_seats_str << endl;
        return {"-1", DateTime(), -1};
    }

    return { eid, dt, num_seats };
}