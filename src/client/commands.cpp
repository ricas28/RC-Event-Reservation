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

void print_help(){
    cout << "Available commands and their usage:\n\n";

    cout << "1. login\n";
    cout << "   - Logs in to the ES via UDP. Registers the UID if it does not exist.\n";
    cout << "   - Usage: login <UID> <password>\n\n";

    cout << "2. changePass\n";
    cout << "   - Changes the user's password via TCP.\n";
    cout << "   - Usage: changePass <oldPassword> <newPassword>\n\n";

    cout << "3. unregister\n";
    cout << "   - Unregisters the currently logged-in user via UDP. Performs logout automatically.\n";
    cout << "   - Usage: unregister\n\n";

    cout << "4. logout\n";
    cout << "   - Logs out the currently logged-in user via UDP.\n";
    cout << "   - Usage: logout\n\n";

    cout << "5. exit\n";
    cout << "   - Exits the application. If a user is logged in, logout first.\n";
    cout << "   - Usage: exit\n\n";

    cout << "6. create\n";
    cout << "   - Creates a new event via TCP. The file event_fname must exist.\n";
    cout << "   - Usage: create <name> <event_fname> <dd-mm-yyyy hh:mm> <num_attendees>\n\n";

    cout << "7. close\n";
    cout << "   - Closes an ongoing event via TCP. Checks event status (expired, sold-out).\n";
    cout << "   - Usage: close <EID>\n\n";

    cout << "8. myevents or mye\n";
    cout << "   - Lists events created by the logged-in user via UDP.\n";
    cout << "   - Usage: myevents | mye\n\n";

    cout << "9. list\n";
    cout << "   - Lists all available events via TCP.\n";
    cout << "   - Usage: list\n\n";

    cout << "10. show\n";
    cout << "    - Shows details of event EID and retrieves the event file via TCP.\n";
    cout << "    - Usage: show <EID>\n\n";

    cout << "11. reserve\n";
    cout << "    - Reserves seats for event EID via TCP. 'value' is the number of seats.\n";
    cout << "    - Usage: reserve <EID> <value>\n\n";

    cout << "12. myreservations or myr\n";
    cout << "    - Lists events the logged-in user has reservations for (up to 50) via UDP.\n";
    cout << "    - Usage: myreservations | myr\n";
}
