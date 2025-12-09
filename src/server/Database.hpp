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

public:
    // Get the only instance
    static Database& instance();

    // ---------- PATH BUILDERS ----------

    // USERS/<uid>/
    std::string user_dir(const std::string& uid) const;

    // USERS/<uid>/pass.txt
    std::string user_pass_file(const std::string& uid) const;

    // USERS/<uid>/login.txt
    std::string user_login_file(const std::string& uid) const;

    // USERS/<uid>/CREATED/
    std::string user_created_dir(const std::string& uid) const;

    // USERS/<uid>/RESERVED/
    std::string user_reserved_dir(const std::string& uid) const;

    // EVENTS/<eid>/
    std::string event_dir(const std::string& eid) const;

    // EVENTS/<eid>/START(eid).txt
    std::string event_start_file(const std::string& eid) const;

    // EVENTS/<eid>/RES(eid).txt
    std::string event_res_file(const std::string& eid) const;

    // EVENTS/<eid>/DESCRIPTION/
    std::string event_desc_dir(const std::string& eid) const;

    // EVENTS/<eid>/DESCRIPTION/<filename>
    std::string event_desc_file(const std::string& eid,
                                const std::string& fname) const;

    // EVENTS/<eid>/END(eid).txt
    std::string event_end_file(const std::string& eid) const;

    // EVENTS/<eid>/RESERVATIONS/
    std::string event_reservations_dir(const std::string& eid) const;

    // EVENTS/<eid>/RESERVATIONS/R-uid-date-time.txt
    std::string event_reservation_file(const std::string& eid,
                                       const std::string& filename) const;

    // ---------- UTILITIES ----------
    bool ensure_user_dirs(const std::string& uid);
    bool ensure_event_dirs(const std::string& eid);
    bool Database::is_ok();
};


#endif