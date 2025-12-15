#ifndef __DB__HPP__
#define __DB__HPP__

#include <string>
#include <vector>

#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"
#include "server.hpp"

using namespace std;

class Database {
private:
    string base_path;   // Folder ./ES_DB/
    string users_path;  // Folder ./ES_DB/USERS/
    string events_path; // Folder ./ES_DB/EVENTS/

    bool ok = false; // Bool for bd initialization

    // Private constructor for Singleton pattern.
    Database();

    // Avoid another instance.
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // ---------- PATH BUILDERS ----------
    // USERS/<uid>/
    string user_dir(const string& uid) const;
    // USERS/<uid>/<uid>_pass.txt
    string user_pass_file(const string& uid) const;
    // USERS/<uid>/<uid>_login.txt
    string user_login_file(const string& uid) const;
    // USERS/<uid>/CREATED/
    string user_created_dir(const string& uid) const;
    // USERS/<uid>/CREATED/<eid>.txt
    string user_created_file(const string &uid, const string &eid) const;
    // USERS/<uid>/RESERVED/
    string user_reserved_dir(const string& uid) const;
    // USERS/<uid>/RESERVED/R-<uid>-<date>-<time>.txt
    string user_reservation_file(const string &uid, DateTime now) const;

    // EVENTS/<eid>/
    string event_dir(const string& eid) const;
    // EVENTS/<eid>/START_(eid).txt
    string event_start_file(const string& eid) const;
    // EVENTS/<eid>/RES_(eid).txt
    string event_res_file(const string& eid) const;
    // EVENTS/<eid>/DESCRIPTION/
    string event_desc_dir(const string& eid) const;
    // EVENTS/<eid>/DESCRIPTION/<filename>
    string event_desc_file(const string& eid,
                                const string& fname) const;
    // EVENTS/<eid>/END_<eid>.txt
    string event_end_file(const string& eid) const;
    // EVENTS/<eid>/RESERVATIONS/
    string event_reservations_dir(const string& eid) const;
    // EVENTS/<eid>/RESERVATIONS/R-<uid>-<date>-<time>.txt
    string event_reservation_file(const string& eid, const string &uid, DateTime now) const;
    //R-uid-date-time.txt
    string reservation_file(const string &uid,  DateTime now) const;

    // ---------- UTILITIES ----------
    bool ensure_user_dirs(const std::string& uid);
    bool ensure_event_dirs(const std::string& eid);
    // Returns the current eid.
    int read_eid(int fd);
    // Increments the current eid.
    bool increment_eid(int fd, int current_eid = -1);
    // Creates a simple file with no text inside.
    bool create_file(const string &path);
    // Deletes a file from the db.
    bool delete_file(const string &path);
    // Writes the content file of the start file.
    bool write_start_file(const string &start_path, const string &uid, Event_creation_Info &event);
    // Writes number of reservations made to the RES file.
    bool write_res_file(const string &end_path, int reservations);
    // Writes content to description file.
    bool write_description_file(const string &description_path, const string &Fdata);

public:
    // Get the only instance
    static Database& instance();

    bool is_ok();

    // ------ GETTERS -------
    size_t get_events_created();

    // ------ HELPERS -------

    // Check if user dir exists.
    bool user_exists(const string &uid);
    // Check for password file.
    bool user_registered(const string &uid);
    // Check if user is logged in.
    bool user_logged_in(const string &uid);
    // Check if an END file exists
    bool is_event_closed(const string &eid);
    // Check if an event exists.
    bool event_exists(const string &eid);
    // Check how many seats were reserved for event.
    int get_reserved_seats(const string &eid);
    // Get the creator of an event.
    bool get_event_creator(const string &eid, string &creator);
    // Checks if an event already happened.
    bool event_passed(const string &eid, bool &in_past);
    // Checks if an event is sold out.
    bool is_event_sold_out(const string &eid, bool &sold_out);
    // Returns the status of an event.
    int get_event_status(const string &eid, StartFileData data);
     // Extracts the info from a START file into a struct.
    StartFileData extract_start_file_data(const string &eid);
    // Extratcs the info from a reservation file into a struct.
    Reservation extract_reservation_file_data(const string &filepath);

    // ----- OPERATIONS -------

    // Create login file for user.
    bool login_user(const string &uid);
    // Writes a password to a user password file.
    bool write_password(const string &uid, const string &pass);
    // Registers a new user
    bool register_user(const string &uid, const string &password);
    // Checks if the given password matches the user password.
    bool check_password(const string &uid, const string &password, bool &equal);
    // Logout a user.
    bool logout_user(const string &uid); 
    // Unregister a uer.
    bool unregister_user(const string &uid);
    // Close an event.
    bool close_event(const std::string &eid, DateTime dt);
    // Get user events.
    void get_user_events(const string &uid,  vector<pair<string, int>> &events);
    // Get user reservations.
    void get_user_reservations(const string &uid, vector<Reservation> &reservations);
    /**
     * Creates an event.
     * 
     * If the event can't be created due to max events reached (999), function
     * returns true and sets 'eid' to '-1'.
     */
    bool create_event(const string &uid, Event_creation_Info &event, string &eid);
    // Returns all of the events on the server.
    bool get_all_events(vector<Event_list> &events);
    // Makes a reservation.
    bool reserve(const string &uid, 
                    const string &eid, 
                    const int &people, 
                    const StartFileData &data,
                    int &remaining_seats);
};


#endif