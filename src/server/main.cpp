#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "parser.hpp"
#include "server.hpp"

using namespace std;

void process_commands(SVArgs &server) {
    fd_set readfds;
    int max_fd = std::max(server.tcp_socket, server.udp_socket);

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server.tcp_socket, &readfds);
        FD_SET(server.udp_socket, &readfds);

        int activity = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr);
        if (activity < 0) {
            if (errno == EINTR) continue; // Ignore signals
            perror("select error");
            break;
        }
        // UDP ready
        if (FD_ISSET(server.udp_socket, &readfds)) {
            handle_udp_request(server.udp_socket);
        }
        // TCP ready
        if (FD_ISSET(server.tcp_socket, &readfds)) {
            struct sockaddr_storage client_addr;
            socklen_t addrlen = sizeof(client_addr);
            int client_fd = accept(server.tcp_socket, (struct sockaddr*)&client_addr, &addrlen);
            if (client_fd < 0) {
                perror("Failure executing accept");
                continue;
            }

            pid_t pid = fork();
            if (pid < 0) {
                perror("Failure executing fork");
                close(client_fd);
            } else if (pid == 0) {
                // Child: Handle tcp request
                destroy_server(server); // Child doesn't need the sockets.
                handle_tcp_request(client_fd);
                close(client_fd);
                _exit(EXIT_SUCCESS);
            } else {
                // Parent: Close client fd.
                close(client_fd);
            }
        }
    }
}

int main(int argc, char **argv){
    string port;
    bool verbose;
    SVArgs server;

    signal(SIGPIPE, SIG_IGN); // Ignore broken pipe.
    signal(SIGCHLD, SIG_IGN); // Avoid zombie processes.

    if(!parse_args(port, verbose, argv, argc)){
        cerr << "Usage: " << argv[0] <<  " [-p ESport] [-v]" << endl;
        exit(EXIT_FAILURE);
    }
    if(server_init(server, port, verbose) == -1){
        cerr << "Failure to initialize server sockets";
        exit(EXIT_FAILURE);
    }
    process_commands(server);
    destroy_server(server);
    exit(EXIT_SUCCESS);
}