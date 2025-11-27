#ifndef __IO__HPP__
#define __IO__HPP__

using namespace std;

/**
 * Reads a line from the given fd. Line can have max size of 
 * BUFFER_SIZE (256).
 * 
 * @param line buffer for storing the command.
 * @param fd File descriptor.
 * 
 * @returns -1 if an error occurs while reading from the fd,
 *           0 if command given exceeds the buffer size,
 *           1 for a successful parse.
 */
int read_line_256(int fd, char *line);

/**
 * Reads dynamically from the given fd.
 * 
 * @param fd File descriptor.
 * @param line String that will contain read content.
 * 
 * @returns Bytes that were
 */
ssize_t read_until_line_end(int fd, string &line);

/**
 * Does a clean up of the given fd
 */
void clean_up_fd(int fd);

/**
 * Writes size bytes from fd into buffer.
 * 
 * @param fd File descriptor.
 * @param buf Buffer with content.
 * @param size Bytes being written.
 * 
 * @return Bytes that were written. 
 */
ssize_t write_all(int fd, const void *buf, size_t size);

/**
 * Reads size bytes into buf from fd.
 * 
 * @param fd File descriptor.
 * @param buf Buffer that stores the content.
 * @param size Bytes being read.
 * 
 * @return Bytes that were read.
 */
ssize_t read_all(int fd, void *buf, size_t size);

/**
 * Reads the content of a file to a buffer.
 * 
 * @param fileName Name of the file.
 * @param out_size Pointer to size of the file that was read.
 * 
 * @returns Pointer to buffer with content that was read.
 */
char *read_file_to_buffer(const char *fileName, size_t *out_size);

/**
 * Writes the content of a buffer to a file.
 * 
 * @param filename Name of the file.
 * @param buffer Buffer with content being written
 * @param size Length being written.
 * 
 * @returns 0 on success, -1 on failure.
 */
int write_buffer_to_file(const char *filename, const void *buffer, size_t size);

#endif