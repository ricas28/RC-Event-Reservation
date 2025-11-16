#include <iostream>
#include <string.h>

using namespace std;

#include "constants.hpp"

void clean_up_stdin(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool parse_line(char *line){
    if(!fgets(line, BUFFER_SIZE, stdin)){
        perror("Failure to read command.");
        return false;
    }
    // End of command wasn't found. Command is too long.
    if (strchr(line, '\n') == NULL) {
        cout << "Command was too long" << endl;
        clean_up_stdin();
        return false;
    }
    return true;
}