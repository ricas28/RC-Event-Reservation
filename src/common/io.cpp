#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

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

size_t read_until_line_end(int fd, string &line){
    line.clear();
    char c;
    bool seen_end = false;

    while (true) {
        ssize_t n = read(fd, &c, 1);
        if (n <= 0) return (size_t)n; // error or EOF

        if (c == '\n')
            seen_end = true;
        line.push_back(c);
        if(seen_end) 
            break;
    }
    return line.size();
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

ssize_t read_all(int fd, void *buf, size_t size){
    size_t total_read = 0;
    char *p = (char *)buf;

    while (total_read < size) {
        ssize_t n = read(fd,p + total_read, (size_t)(size - total_read));

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

char *read_file_to_buffer(const char *fileName, size_t *out_size) {
    FILE *f = fopen(fileName, "rb");
    if (!f) {
        perror("Failure to open file");
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        perror("Failure to go to end of file");
        fclose(f);
        return NULL;
    }

    long file_size = ftell(f);
    if (file_size < 0) {
        perror("Failure to check size of file");
        fclose(f);
        return NULL;
    }

    rewind(f);

    // Conversion to size_t
    size_t size = (size_t)file_size;
    char *buffer = (char*)malloc(size);
    if (!buffer) {
        perror("Failure to allocate memory for buffer");
        fclose(f);
        return NULL;
    }

    size_t n = fread(buffer, 1, size, f);
    fclose(f);

    if (n != size) {
        cerr << "Whole file wasn't read" << endl;
        free(buffer);
        return NULL;
    }

    *out_size = size;
    return buffer;

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

