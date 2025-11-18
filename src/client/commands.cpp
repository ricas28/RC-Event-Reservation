#include <iostream>

#include "../common/DateTime.hpp"
#include "parser.hpp"

using namespace std;

/**
 * Checks if a user is logged in. Printing a message if not.
 */
bool is_logged_in(bool logged_in){
    if(!logged_in){
        cout << "User not logged in" << endl;
        return false;
    }
    return true;
}

void handle_login(char *args, bool *logged_in){
    int uid; 
    string pass;
    if (parse_login(args, &uid, &pass)){
        *logged_in = true;
    }
}

void handle_change_pass(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    string old_pass, new_pass;
    if (parse_change_pass(args, &old_pass, &new_pass)){
        //er_change_pass();
    }
}

void handle_unregister(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    if (parse_unregister(args)){
        //er_unregister();
    }
}

void handle_logout(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    if (parse_logout(args)){
        *logged_in = false;
    }
}

void handle_exit(char *args, bool *logged_in, bool *quit){
    if (parse_exit(args)){
        if (*logged_in){
            cout << "Currently logged in!" << endl;
            cout << "Please execute the 'logout' command first" << endl;
        } else {
            *quit = true;
        }
    }
}

void handle_create(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    string name, event_fname;
    DateTime event_date;
    int num_attendees;

    if (parse_create(args, &name, &event_fname, &event_date, &num_attendees)){
        //er_create();
    }
}

void handle_close(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    int eid;
    if (parse_close(args, &eid)){
        //er_close();
    }
}

void handle_myevents(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    if (parse_myevents(args)){
        //er_myevents();
    }
}

void handle_list(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    if (parse_list(args)){
        //er_list();
    }
}

void handle_show(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    int eid;
    if (parse_show(args, &eid)){
        //er_show();
    }
}

void handle_reserve(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    int eid, seats;
    if (parse_reserve(args, &eid, &seats)){
        //er_reserve();
    }
}

void handle_myreservations(char *args, bool *logged_in){
    if (!is_logged_in(*logged_in)) return;

    if (parse_myreservations(args)){
        //er_myreservations();
    }
}
