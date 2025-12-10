#ifndef __DB__HPP__
#define __DB__HPP__

#include <string>

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
    // USERS/<uid>/pass.txt
    string user_pass_file(const string& uid) const;
    // USERS/<uid>/login.txt
    string user_login_file(const string& uid) const;
    // USERS/<uid>/CREATED/
    string user_created_dir(const string& uid) const;
    // USERS/<uid>/RESERVED/
    string user_reserved_dir(const string& uid) const;
    // EVENTS/<eid>/
    string event_dir(const string& eid) const;
    // EVENTS/<eid>/START(eid).txt
    string event_start_file(const string& eid) const;
    // EVENTS/<eid>/RES(eid).txt
    string event_res_file(const string& eid) const;
    // EVENTS/<eid>/DESCRIPTION/
    string event_desc_dir(const string& eid) const;
    // EVENTS/<eid>/DESCRIPTION/<filename>
    string event_desc_file(const string& eid,
                                const string& fname) const;
    // EVENTS/<eid>/END(eid).txt
    string event_end_file(const string& eid) const;
    // EVENTS/<eid>/RESERVATIONS/
    string event_reservations_dir(const string& eid) const;
    // EVENTS/<eid>/RESERVATIONS/R-uid-date-time.txt
    string event_reservation_file(const string& eid, const string& filename) const;
    // ---------- UTILITIES ----------
    bool ensure_user_dirs(const std::string& uid);
    bool ensure_event_dirs(const std::string& eid);

public:
    // Get the only instance
    static Database& instance();

    bool is_ok();

    // Check if user dir exists.
    bool user_exists(const string &uid);
    // Check for password file.
    bool user_registered(const string &uid);
    // Check if user is logged in.
    bool user_logged_in(const string &uid);
    // Create login file for user.
    bool login_user(const string &uid);
    // Registers a new user
    bool register_user(const string &uid, const string &password);
    // Checks if the given password matches the user password.
    bool check_password(const string &uid, const string &password);
    // Logout a user.
    bool logout_user(const string &uid); 
};


#endif