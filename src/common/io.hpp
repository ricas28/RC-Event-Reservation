#ifndef __IO__HPP__
#define __IO__HPP__

/**
 * Parses a line from the stdin. Line has a size of BUFFER_SIZE (256).
 * 
 * @param line buffer for storing the command.
 * 
 * @returns -1 if an error occurs while readinf from the stdin,
 *           0 if command given exceeds the buffer size,
 *           1 for a successful parse.
 */
int parse_line(char *line);

/**
 * Does a clean up of the stdin.
 */
void clean_up_stdin();

#endif