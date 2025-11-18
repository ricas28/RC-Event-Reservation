#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace std;

#include "constants.hpp"

void clean_up_fd(int fd){
    int c;
    while (read(fd, &c, 1) == 1 && c != '\n');
}

int read_line(int fd, char *line){
    int idx = 0;
    char c;

    while (idx < BUFFER_SIZE - 1) {
        ssize_t r = read(fd, &c, 1);
        if (r == -1) {
            perror("Failure to read command.");
            return -1;
        }
        if (r == 0) {  // EOF
            if (idx == 0)
                return 0; // nothing read
            break;
        }
        line[idx++] = c;
        if (c == '\n') break;
    }

    line[idx] = '\0';
    // Detect if line was too long (didn't end in \n and still space in buffer)
    if (idx == BUFFER_SIZE - 1 && line[idx - 1] != '\n') {
        cout << "Command was too long!" << endl;
        clean_up_fd(fd);
        return 0;
    }
    return 1;
}