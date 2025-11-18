#ifndef __IO__HPP__
#define __IO__HPP__

/**
 * Parses a line from the given fd. Line has a size of BUFFER_SIZE (256).
 * 
 * @param line buffer for storing the command.
 * @param fd File descriptor.
 * 
 * @returns -1 if an error occurs while reading from the fd,
 *           0 if command given exceeds the buffer size,
 *           1 for a successful parse.
 */
int read_line(int fd, char *line);

/**
 * Does a clean up of the given fd
 */
void clean_up_fd(int fd);

#endif