#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "parser.hpp"
#include "server.hpp"
#include "operations.hpp"

using namespace std;

int _udp_socket = -1;
int _tcp_socket = -1;

void process_commands(int verbose) {
    fd_set readfds;
    int max_fd = max(_tcp_socket, _udp_socket);

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(_tcp_socket, &readfds);
        FD_SET(_udp_socket, &readfds);

        int activity = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            if (errno == EINTR) continue; // Ignore signals
            perror("select error");
            break;
        }
        // UDP ready
        if (FD_ISSET(_udp_socket, &readfds)) {
            if(handle_udp_request(_udp_socket, verbose) == -1){

            }
        }
        // TCP ready
        if (FD_ISSET(_tcp_socket, &readfds)) {
            struct sockaddr_in client_addr;
            socklen_t addrlen = sizeof(client_addr);
            int client_fd = accept(_tcp_socket, (struct sockaddr*)&client_addr, &addrlen);
            if (client_fd < 0) {
                perror("Failure executing accept");
                continue;
            }

            pid_t pid = fork();
            if (pid < 0) {
                perror("Failure executing fork");
                close(client_fd);
            } else if (pid == 0) {
                // Child: Handle TCP request
                destroy_server(_udp_socket, _tcp_socket); // Child doesn't need the sockets.
                handle_tcp_request(client_fd, client_addr, verbose);
                close(client_fd);
                _exit(EXIT_SUCCESS);
            } else {
                // Parent: Close client fd.
                close(client_fd);
            }
        }
    }
}

void handle_sigint(int signum){
    (void) signum; // To avoid warnings.
    destroy_server(_tcp_socket, _udp_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv){
    string port;
    bool verbose;

    signal(SIGPIPE, SIG_IGN); // Ignore broken pipe.
    signal(SIGCHLD, SIG_IGN); // Avoid zombie processes.
    signal(SIGINT, handle_sigint); // End program softly.

    if(db_init() == -1)
        exit(EXIT_FAILURE);    

    if(!parse_args(port, verbose, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-p ESport] [-v]" << endl;
        exit(EXIT_FAILURE);
    }
    if(server_init(port, _tcp_socket, _udp_socket) == -1){
        cerr << "Failure to initialize server sockets" << endl;
        exit(EXIT_FAILURE);
    }
    process_commands(verbose);
    destroy_server(_tcp_socket, _udp_socket);
    exit(EXIT_SUCCESS);
}