#include <iostream>

#include "../common/DateTime.hpp"
#include "parser.hpp"
#include "Client.hpp"
#include "api.hpp"

using namespace std;

/**
 * Checks if a user is logged in. Printing a message if not.
 */
bool is_logged_in(bool logged_in){
    if(!logged_in){
        cout << "user not logged in" << endl;
        return false;
    }
    return true;
}

void handle_login(char *args, CLArgs *client){
    if(client->logged_in){
        cout << "user already logged in" << endl;
        return;
    }
    int uid;
    string pass;
    if (parse_login(args, &uid, &pass)){
        if(er_login(client, uid, pass) == -1){
            cerr << "Failure to execute 'login' command..." << endl;
            return;
        }
    }
}

void handle_changePass(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    string old_pass, new_pass;
    if (parse_changePass(args, &old_pass, &new_pass)){
        if(er_changePass(*client, old_pass, new_pass) == -1){
            cerr << "Failure to execute 'changePass' command..." << endl;
        }
    }
}

void handle_unregister(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    if (parse_unregister(args)){
         if(er_unregister(client) == -1){
            cerr << "Failure to execute 'unregister' command..." << endl;
        }
    }
}

void handle_logout(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    if (parse_logout(args)){
        if(er_logout(client) == -1){
            cerr << "Failure to execute 'logout' command..." << endl;
        }
    }
}

void handle_exit(char *args, CLArgs *client, bool *quit){
    if (parse_exit(args)){
        if (client->logged_in){
            cout << "Currently logged in!" << endl;
            cout << "Please execute the 'logout' command first" << endl;
        } else {
            *quit = true;
        }
    }
}

void handle_create(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    string name, event_fname;
    DateTime event_date;
    ssize_t Fsize;
    int num_attendees;

    if (parse_create(args, &name, &event_fname, &Fsize, &event_date, &num_attendees)){
        if(er_create(*client, name, event_fname, Fsize, event_date, num_attendees) == -1){
            cerr << "Failure to execute 'create' command..." << endl;
        }
    }
}

void handle_close(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    string eid;
    if (parse_close(args, eid)){
        if(er_close(*client, eid) == -1){
            cerr << "Failure to execute 'close' command..." << endl;
        }
    }
}

void handle_myevents(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    if (parse_myevents(args)){
        if(er_myevents(*client) == -1){
            cerr << "Failure to execute 'myevents' command..." << endl;
        }
    }
}

void handle_list(char *args, CLArgs *client){
    if (parse_list(args)){
        if(er_list(*client) == -1){
            cerr << "Failure to execute 'list' command..." << endl;
        }
    }
}

void handle_show(char *args, CLArgs *client){
    string eid;
    if (parse_show(args, eid)){
        if(er_show(*client, eid) == -1)
            cerr << "Failure to exeute 'show' command..." << endl;
    }
}

void handle_reserve(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    string eid;
    int seats;
    if (parse_reserve(args, eid, seats)){
        if(er_reserve(*client, eid, seats) == -1)
            cerr << "Failure to execute 'reserve' command..." << endl;
    }
}

void handle_myreservations(char *args, CLArgs *client){
    if (!is_logged_in(client->logged_in)) return;

    if (parse_myreservations(args)){
        if(er_myreservations(*client) == -1){
            cerr << "Failure to execute 'myreservations' command..." << endl;
        }
    }
}
