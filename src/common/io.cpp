#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <stdbool.h>

using namespace std;

#include "constants.hpp"

void clean_up_fd(int fd){
    int c;
    while (read(fd, &c, 1) == 1 && c != '\n');
}

int read_line_256(int fd, char *line){
    int idx = 0;
    char c;

    while (idx < BUFFER_SIZE - 1) {
        ssize_t r = read(fd, &c, 1);
        if (r == -1) {
            perror("Failure to read line.");
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

bool open_and_lock(const string &path, int flags, int lock_type, int &fd){
    fd = open(path.c_str(), flags, 0644);
    if (fd < 0) {
        perror(("open: " + path).c_str());
        return false;
    }

    if (flock(fd, lock_type) < 0) {
        perror(("flock: " + path).c_str());
        close(fd);
        return false;
    }

    return true;
}

ssize_t read_all(int fd, void *buf, size_t size){
    size_t total_read = 0;
    char *p = (char *)buf;

    while (total_read < size) {
        ssize_t n = read(fd, p + total_read, (size_t)(size - total_read));

        if (n < 0) {
            if (errno == EINTR)
                continue; // Try again if write is interrupted
            return -1;   // Real error.
        }

        if (n == 0) {
            return (ssize_t)total_read;  // EOF
        }
        total_read += (size_t)n;
    }
    return (ssize_t)total_read;
}

ssize_t write_all(int fd, const void *buf, size_t size){
    size_t total_written = 0;
    const char *p = (const char *)buf;

    while (total_written < size) {
        ssize_t n = write(fd, p + total_written, (size_t)(size - total_written));

        if (n < 0) {
            if (errno == EINTR)
                continue; // Try again if write is interrupted
            return -1; // Real error.
        }
        if (n == 0) {
            return (ssize_t)total_written;
        }
        total_written += (size_t)n;
    }
    return (ssize_t)total_written;
}

int write_buffer_to_file(const char *filename, const void *buffer, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failure to open file");
        return -1;
    }

    size_t written = fwrite(buffer, 1, size, file);
    if (written != size) {
        perror("Failure to write to file");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;  // success.
}

bool get_file_size(const char *filename, size_t *size){
    struct stat st;

    if (stat(filename, &st) != 0)
        return false;

    // GMake sure it's a regular file.
    if (!S_ISREG(st.st_mode))
        return false;

    *size = (size_t) st.st_size;
    return true;
}
