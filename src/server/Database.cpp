#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>           
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <cstring>   
#include <cerrno>
#include <sys/stat.h>

#include "../common/util.hpp"
#include "../common/io.hpp"
#include "../common/DateTime.hpp"
#include "../common/protocol.hpp"
#include "../common/constants.hpp"
#include "server.hpp"
#include "Database.hpp"

using std::string;
namespace fs = std::filesystem;

#define DATABASE_FOLDER "ES_DB"
#define USERS_FOLDER "USERS"
#define EVENTS_FOLDER "EVENTS"
#define EID_COUNTER "eid_counter.txt"

Database::Database(): ok(true) {
    base_path   = "./" + string(DATABASE_FOLDER) + "/";
    users_path  = base_path + string(USERS_FOLDER) + "/";
    events_path = base_path + string(EVENTS_FOLDER) + "/";

    if(!safe_create_dir(users_path) || !safe_create_dir(events_path))
        ok = false;

    string counter_file = base_path + EID_COUNTER;
    ifstream infile(counter_file);
    if (!infile.is_open()) {
        // File does not exist -> create it with 0
        ofstream outfile(counter_file);
        if (!outfile.is_open()) {
            cerr << "Failed to create counter file: " << counter_file << endl;
        } else {
            outfile << 0 << endl;
            outfile.close();
        }
    }
}

Database& Database::instance() {
    static Database db;
    return db;
}

bool Database::is_ok() { return ok; }

// ----------------- USERS -----------------
string Database::user_dir(const string& uid) const {
    return users_path + uid + "/";
}

string Database::user_pass_file(const string& uid) const {
    return user_dir(uid) + uid + "_pass.txt";
}

string Database::user_login_file(const string& uid) const {
    return user_dir(uid) + uid + "_login.txt";
}

string Database::user_created_dir(const string& uid) const {
    return user_dir(uid) + "CREATED/";
}

string Database::user_created_file(const string &uid, const string &eid) const {
    return user_created_dir(uid) + eid + ".txt";
}

string Database::user_reserved_dir(const string& uid) const {
    return user_dir(uid) + "RESERVED/";
}

string Database::user_reservation_file(const string &uid,  DateTime now) const {
    return user_reserved_dir(uid) + reservation_file(uid, now);
}

// Ensure structure: USERS/<uid>/(pass/login/CREATED/RESERVED)
bool Database::ensure_user_dirs(const string& uid) {
    // Create USER dir.
    if (!safe_create_dir(user_dir(uid))) return false;

    // Create CREATED and RESERVED subdirs.
    if (!safe_create_dir(user_created_dir(uid)))  return false;
    if (!safe_create_dir(user_reserved_dir(uid))) return false;

    return true;
}

// ----------------- EVENTS -----------------
string Database::event_dir(const string& eid) const {
    return events_path + eid + "/";
}

string Database::event_start_file(const string& eid) const {
    return event_dir(eid) + "START_" + eid + ".txt";
}

string Database::event_res_file(const string& eid) const {
    return event_dir(eid) + "RES_" + eid + ".txt";
}

string Database::event_desc_dir(const string& eid) const {
    return event_dir(eid) + "DESCRIPTION/";
}

string Database::event_desc_file(const string& eid, const string& fname) const {
    return event_desc_dir(eid) + fname;
}

string Database::event_end_file(const string& eid) const {
    return event_dir(eid) + "END_" + eid + ".txt";
}

string Database::event_reservations_dir(const string& eid) const {
    return event_dir(eid) + "RESERVATIONS/";
}

string Database::event_reservation_file(const string& eid, const string& uid, DateTime now) const {
    return event_reservations_dir(eid) + reservation_file(uid, now);
}

string Database::reservation_file(const string &uid, DateTime now) const {
    return "R-" + uid + "-" + now.DatetoString(true) + "-" + now.TimetoString(true) + ".txt";
}

bool Database::ensure_event_dirs(const string& eid) {
    // Create EVENTS dir
    if (!safe_create_dir(event_dir(eid))) return false;

    // Create DESCRIPTION and RESERVATIONS subdirs.
    if (!safe_create_dir(event_desc_dir(eid)))          return false;
    if (!safe_create_dir(event_reservations_dir(eid)))  return false;

    return true;
}

bool Database::increment_eid(int fd, int current_eid){
    int eid = 0;
    // Read current eid value if necessary.
    if(current_eid == -1){
        eid = read_eid(fd);
        eid++;
    }
    else{
        eid = current_eid + 1;
    }

    // Go back to the beginning of the file.
    lseek(fd, 0, SEEK_SET);
    if (ftruncate(fd, 0) == -1) {
        perror("ftruncate failed");
        close(fd);
        return false;
    }

    string eid_str =  to_string(eid);
    if(write_all(fd, eid_str.c_str(), eid_str.size()) <= 0){
        cerr << "failure to write to EID counter file" << endl;
        return false;
    }

    return true;
}

int Database::read_eid(int fd){
    char eid_str[EID_SIZE];

    if(read_all(fd, eid_str, EID_SIZE-1) <= 0){
        cerr << "Failure to read from EID counter file" << endl;
        return -1;
    }

    return atoi(eid_str);
}

bool Database::create_file(const string &path){
    int fd;
    if(!open_and_lock(path, O_CREAT | O_WRONLY, LOCK_EX, fd))
        return false;

    close(fd);  // Unlock
    return true;
}

bool Database::delete_file(const string &path){
    int fd;
    // Optional: try to lock before deleting
    if(open_and_lock(path, O_WRONLY, LOCK_EX, fd)){
        if(unlink(path.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink failed for file " + path).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    } else {
        // Could not lock -> maybe file doesn't exist
        if(errno != ENOENT){
            perror(("Failed to lock file before deleting: " + path).c_str());
            return false;
        }
    }
    return true;
}

bool Database::write_start_file(const string &start_path, const string &uid, Event_creation_Info &event){
    int fd;
    if(!open_and_lock(start_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string content = uid + " " + event.name + " " + event.Fname + " " +
                          to_string(event.attendace_size) + " " +
                          event.event_date.toString() + "\n";

    if(write_all(fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        perror(("write_all: " + start_path).c_str());
        delete_file(start_path);
        close(fd);
        return false;
    }

    close(fd); // unlock + close
    return true;
}

bool Database::write_res_file(const string &res_path, int reservations){
    int fd;
    if(!open_and_lock(res_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string content = to_string(reservations) + "\n";

    if(write_all(fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        perror(("write_all: " + res_path).c_str());
        delete_file(res_path);
        close(fd);
        return false;
    }

    close(fd); // unlock + close
    return true;
}

bool Database::write_description_file(int &sock, const string &description_path, const ssize_t Fsize){
    int fd;
    if(!open_and_lock(description_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    if(stream_file_TCP(sock, fd, (size_t)Fsize) != Fsize){
        perror(("write_all: " + description_path).c_str());
        unlink(description_path.c_str());
        close(fd);
        return false;
    }

    close(fd); // unlock + close
    return true;
}

bool Database::user_exists(const string &uid){
    return fs::exists(user_dir(uid));
}

bool Database::user_registered(const string &uid){
    return fs::exists(user_pass_file(uid));
}

bool Database::user_logged_in(const string &uid){
    return fs::exists(user_login_file(uid));
}

bool Database::is_event_closed(const string &eid){
    return fs::exists(event_end_file(eid));
}

bool Database::event_exists(const string &eid){
    return fs::exists(event_dir(eid));
}

bool Database::get_event_creator(const string &eid, string &creator){
    StartFileData data = extract_start_file_data(eid);
    if(data.uid == -1)
        return false;

    creator = to_string(data.uid);
    return true;
}

bool Database::event_passed(const string &eid, bool &in_past){
    StartFileData data = extract_start_file_data(eid);
    if(data.uid == -1)
        return false;

    in_past = data.start_date_time.isPast();
    return true;
}

bool Database::is_event_sold_out(const string &eid, bool &sold_out){
    StartFileData data = extract_start_file_data(eid);
    if(data.uid == -1)
        return false;

    int reserved = get_reserved_seats(eid);
    if(reserved == -1)
        return false;

    sold_out = reserved >= data.event_attend;
    return true;
}

int Database::get_event_status(const string &eid, StartFileData data){
    // Check for invalid values (extraction failed).
    if(data.uid == -1){
        return EVENT_CORRUPTED;
    }

    // Check if event already occured.
    if(data.start_date_time.isPast()){
        // Close event
        close_event(eid, data.start_date_time);
        return EVENT_IN_PAST;
    }

    // Check if event is closed.
    if(is_event_closed(eid)){
        return EVENT_CLOSED;
    }

    // Check if event is sold out or not.
    int reserved = get_reserved_seats(eid);
    if(reserved == -1){
        return EVENT_CORRUPTED;
    }
    int event_status = (reserved >= data.event_attend) ? EVENT_SOLD_OUT : EVENT_ACCEPTING;
    return event_status;
}

bool validateStartFileData(string filepath,
                            string uid, 
                            string event_name, 
                            string desc_fname, 
                            string start_date, 
                            string start_time,
                            DateTime &dt,
                            int event_attend){
    // Validate info
    if(!is_valid_userid((char *)uid.c_str())){
        cerr << "Invalid UID on file  '" << filepath << "': " << uid << endl;
        return false;;
    }
    if(!is_valid_event_name((char *)event_name.c_str())){
        cerr << "Invalid event name on file: '" << filepath << "': " << event_name << endl;
        return false;
    }
    if(!is_valid_file_name((char *)desc_fname.c_str())){
        cerr << "Invalid file name on file: '" << filepath << "': " << desc_fname << endl;
        return false;
    }
    if(!is_valid_num_attendees(event_attend)){
        cerr << "Invalid number of attendees no file '" << filepath << "': " << event_attend << endl;
        return false;
    }
    if(!DateTime::fromStrings(start_date, start_time, dt)){
        cerr << "Invalid date or time on file '" << filepath << "': " 
                                    << start_date << " " << start_time << endl;
        return false;
    }
    return true;
}

StartFileData Database::extract_start_file_data(const string &eid){
    string filepath = event_start_file(eid);
    int fd;
    if(!open_and_lock(filepath, O_RDONLY, LOCK_SH, fd))
        return { -1, "", "", -1, DateTime() };

    // Get file size.
    struct stat st;
    if(fstat(fd, &st) == -1){
        perror(("fstat: " + filepath).c_str());
        close(fd);
        return { -1, "", "", -1, DateTime() };
    }

    vector<char> buffer((size_t)st.st_size + 1, 0);
    if(read_all(fd, buffer.data(), (size_t)st.st_size) != st.st_size){
        perror(("read_all: " + filepath).c_str());
        close(fd);
        return { -1, "", "", -1, DateTime() };
    }
    close(fd); // unlock + close

    istringstream iss(buffer.data());
    string uid, event_name, desc_fname, start_date, start_time;
    int event_attend;

    if(!(iss >> uid >> event_name >> desc_fname >> event_attend >> start_date >> start_time)){
        cerr << "Invalid start file format: " << filepath << endl;
        return { -1, "", "", -1, DateTime() };
    }

    DateTime dt;
    if(!validateStartFileData(filepath, uid, event_name, desc_fname, start_date, start_time, dt, event_attend)){
        return { -1, "", "", -1, DateTime() };
    }

    return {atoi(uid.c_str()), event_name, desc_fname, event_attend, dt};
}

Reservation Database::extract_reservation_file_data(const string &filepath){
    int fd;
    if(!open_and_lock(filepath, O_RDONLY, LOCK_SH, fd))
        return {"-1", DateTime(), -1};

    // Get file size.
    struct stat st;
    if(fstat(fd, &st) == -1){
        perror(("fstat: " + filepath).c_str());
        close(fd);
        return {"-1", DateTime(), -1};
    }

    vector<char> buffer((size_t)st.st_size + 1, 0);
    if(read_all(fd, buffer.data(), (size_t)st.st_size) != st.st_size){
        perror(("read_all: " + filepath).c_str());
        close(fd);
        return {"-1", DateTime(), -1};
    }
    close(fd); // unlock + close

    istringstream iss(buffer.data());
    string eid, date_str, time_str, num_seats_str;
    int num_seats;

    if(!(iss >> eid >> num_seats_str >> date_str >> time_str)){
        cerr << "Invalid format on file: " << filepath << endl;
        return {"-1", DateTime(), -1};
    }

    if(!is_valid_eid((char *)eid.c_str())){
        cerr << "Invalid EID on file '" << filepath  << "': "<< eid << endl;
        return {"-1", DateTime(), -1};
    }

    DateTime dt;
    if(!DateTime::fromStrings(date_str, time_str, dt)){
        cerr << "Invalid date or time on file: '" << filepath << "': " << date_str << " " << time_str << endl;
        return {"-1", DateTime(), -1};
    }

    if(!is_valid_seats((char *)num_seats_str.c_str(), &num_seats)){
        cerr << "Invalid number of reserved seats on file '" << filepath << "': " << num_seats_str << endl;
        return {"-1", DateTime(), -1};
    }

    return { eid, dt, num_seats };
}

// ----------- OPERATIONS ---------------

bool Database::login_user(const string &uid){
    const string login_file = user_login_file(uid);
    return create_file(login_file);  // Already locks in create_file
}

bool Database::write_password(const string &uid, const string &password){
    string pass_file = user_pass_file(uid);
    int fd;
    if(!open_and_lock(pass_file, O_CREAT | O_WRONLY | O_TRUNC, LOCK_EX, fd))
        return false;

    string data = password + "\n";
    if(write_all(fd, data.c_str(), data.size()) != (ssize_t)data.size()){
        perror(("Failed to write password to " + pass_file).c_str());
        close(fd);
        return false;
    }

    close(fd); // Release lock
    return true;
}

bool Database::register_user(const string &uid, const string &password) {
    if (!ensure_user_dirs(uid)) return false;

    if(!write_password(uid, password))
        return false;

    // Create login file
    if(!login_user(uid))
        return false;

    return true;
}

bool Database::check_password(const string &uid, const string &password, bool &equal){
    string pass_file = user_pass_file(uid);
    int fd;
    if(!open_and_lock(pass_file, O_RDONLY, LOCK_SH, fd))
        return false;

    char buf[PASS_SIZE];  
    ssize_t n = read_all(fd, buf, PASS_SIZE - 1);
    close(fd);

    if(n <= 0) return false;
    string stored(buf, (size_t)n);
    stored.erase(stored.find_last_not_of("\r\n")+1); // Remove newline

    equal = stored == password;
    return true;
}

bool Database::logout_user(const string &uid){
    const string login_file = user_login_file(uid);
    int fd;
    if(open_and_lock(login_file, O_WRONLY, LOCK_EX, fd)){
        if(unlink(login_file.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink for 'logout' failed: " + login_file).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    }
    return true;   
}

bool Database::unregister_user(const string &uid){
    const string pass_file = user_pass_file(uid);
    int fd;

    if(open_and_lock(pass_file, O_WRONLY, LOCK_EX, fd)){
        if(unlink(pass_file.c_str()) == -1){
            if(errno != ENOENT){
                perror(("unlink failed for password file: " + pass_file).c_str());
                close(fd);
                return false;
            }
        }
        close(fd);
    }

    // Remove login file
    if(!logout_user(uid))
        return false;
    return true;
}

bool Database::close_event(const std::string &eid, DateTime dt){
    string end_file_path = event_end_file(eid);
    int fd;

    // Open and exclusively lock the file
    if(!open_and_lock(end_file_path, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, fd))
        return false;

    string data = dt.toString(true) + "\n";
    if(write_all(fd, data.c_str(), data.size()) != (ssize_t)data.size()){
        cerr << "Failed to write closing date to END file: " << end_file_path << endl;
        close(fd); // unlocks on close
        return false;
    }

    close(fd); // releases lock
    return true;
}

int Database::get_reserved_seats(const string &eid){
    string reservations_file = event_res_file(eid);
    int fd;

    // Open and shared lock for reading
    if(!open_and_lock(reservations_file, O_RDONLY, LOCK_SH, fd))
        return -1;

    char buf[64];
    ssize_t n = read_all(fd, buf, sizeof(buf)-1);
    close(fd); // releases lock

    if(n <= 0){
        cerr << "Failed to read reservations from file for event " << eid << endl;
        return -1;
    }

    int reservations = atoi(buf);
    return reservations;
}

void Database::get_user_events(const string &uid,  vector<pair<string, int>> &events){
    events.clear();
    string user_events_path = user_created_dir(uid);

    // Iterate through files on the CREATED dir.
    for (const auto& entry : fs::directory_iterator(user_events_path)) {
        if (!entry.is_regular_file())
            continue;
        // Extract the eid of the event for each file.
        string eid = entry.path().stem().string();

        StartFileData data = extract_start_file_data(eid);

        // Get status of events
        events.push_back({eid, get_event_status(eid, data)});
    }

    // Sort by EID.
    sort(events.begin(), events.end(),
     [](const pair<string, int> &a, const pair<string, int> &b) {
         return stoi(a.first) < stoi(b.first);
     });
}

void Database::get_user_reservations(const string &uid, vector<Reservation> &reservations){
    reservations.clear();
    string reservations_path = user_reserved_dir(uid);

    vector<string> files;
    for (const auto &entry : fs::directory_iterator(reservations_path)) {
        if (!entry.is_regular_file())
            continue;
        files.push_back(entry.path().string());
    }

    // Sort by name.
    sort(files.begin(), files.end(), greater<string>());

    for (size_t i = 0; i < MAX_RESERVATIONS_SEND && i < files.size(); i++) {
        Reservation r = extract_reservation_file_data(files[i]);
        reservations.push_back(r);
    }
}

bool Database::create_event(int sock, const string &uid, Event_creation_Info &event, string &eid){
    const string counter_path = base_path + EID_COUNTER;
    int fd;

    // Open and lock counter file for EID.
    if (!open_and_lock(counter_path, O_RDWR | O_CREAT, LOCK_EX, fd)) {
        return false;
    }

    int events_created = read_eid(fd);
    if(events_created == -1){
        close(fd);
        return false;
    }
    if(events_created == MAX_EVENTS){
        eid = "-1";
        close(fd);
        return false;
    }

    eid = format_eid(events_created + 1);
    if(!ensure_event_dirs(eid)){
        close(fd);
        return false;
    }
    string event_file = user_created_file(uid, eid);
    if(!create_file(event_file)){
        cerr << "Failure to open file on CREATED dir" << endl;
        close(fd);
        return false;
    } 

    string start_path = event_start_file(eid);
    string res_path = event_res_file(eid);
    string description_path = event_desc_file(eid, event.Fname);

    if(!write_start_file(start_path, uid, event)){
        delete_file(event_file);
        close(fd);
        return false;
    }

    if(!write_res_file(res_path, 0)){
        delete_file(event_file);
        delete_file(start_path);
        close(fd);
        return false;
    }

    if(!write_description_file(sock, description_path, event.Fsize)){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    // Make sure the message sent ends with '\n'
    char c;
    if(read(sock, &c, 1)  <= 0){
        cerr << "Failure to read ending of protocol message" << endl;
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    if(c != '\n'){
        cerr << "'create' request didn't end with a '\\n'" << endl;
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    // Increase EID counter.
    if(!increment_eid(fd, events_created)){
        delete_file(event_file);
        delete_file(start_path);
        delete_file(res_path);
        close(fd);
        return false;
    }
    close(fd);
    return true;
 }

bool Database::get_all_events(vector<Event_list> &events) {
    events.clear();
    vector<pair<int, string>> eids; // (numeric eid, string eid)

    // Search through every event and store numeric eid (for sorting).
    for (const auto &entry : fs::directory_iterator(events_path)) {
        if (!entry.is_directory() && !entry.is_regular_file())
            continue;

        string name = entry.path().filename().string();

        // EID must have 3 digits.
        if (name.size() != 3 || !isdigit(name[0]) || !isdigit(name[1]) || !isdigit(name[2]))
            continue;

        int eid_num = stoi(name);
        eids.emplace_back(eid_num, name);
    }

    // Sort by EID number (if we sort with the original EID results are not correct).
    sort(eids.begin(), eids.end(),
         [](const auto &a, const auto &b) {
             return a.first < b.first;
         });

    // Process sorted EID's
    for (const auto &[eid_num, eid] : eids) {
        StartFileData data = extract_start_file_data(eid);
        int status = get_event_status(eid, data);

        if (status == EVENT_CORRUPTED) {
            events.push_back({eid, "-1", status, DateTime()});
        } else {
            events.push_back({eid, data.event_name, status, data.start_date_time});
        }
    }
    return true;
}


bool Database::show_event(string &eid, Event_show_Info &data, string &filepath){
    // Get event data.
    StartFileData start_data = extract_start_file_data(eid);
    if(start_data.uid == -1)
        return false;

    // Get number of reserved seats.
    int seats_reserved = get_reserved_seats(eid);
    if(seats_reserved == -1)
        return false;

    // Get the file size.
    size_t Fsize;
    if(!get_file_size(event_desc_file(eid, start_data.desc_fname).c_str(), &Fsize))
        return false;

    // Store data.
    filepath = event_desc_file(eid, start_data.desc_fname);
    data = {
        start_data.uid, 
        start_data.event_name, 
        start_data.start_date_time,
        start_data.event_attend,
        seats_reserved,
        start_data.desc_fname,
        Fsize
    };
    return true;
}

bool Database::reserve(const string &uid, 
                        const string &eid, 
                        const int &people, 
                        const StartFileData &data,
                        int &remaining_seats){
    DateTime dateTime = DateTime::now();
    string events_reservations = event_reservation_file(eid, uid, dateTime);
    string user_reservations = user_reservation_file(uid, dateTime);

    string content = eid + " " + to_string(people) + " " + dateTime.toString(true);

    if(data.uid == -1)
        return false;

    // check if there are enough seats
    int reserved_seats = get_reserved_seats(eid);
    if(reserved_seats == -1)
        return false;

    if(reserved_seats + people > data.event_attend){
        remaining_seats = data.event_attend - reserved_seats;
        return true;
    }

    int user_fd, event_fd;
    // Try to open and lock user file.
    if (!open_and_lock(user_reservations, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, user_fd))
        return false;

    if(write_all(user_fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        cerr << "Failute to write content to user reservation file" << endl;
        delete_file(user_reservations);
        close(user_fd);
        return false;
    }
    close(user_fd);

    // Try to open and lock event file.
    if (!open_and_lock(events_reservations, O_WRONLY | O_CREAT | O_TRUNC, LOCK_EX, event_fd)){
        delete_file(user_reservations);
        return false;
    }

    if(write_all(event_fd, content.c_str(), content.size()) != (ssize_t)content.size()){
        cerr << "Failute to write content to user reservation file" << endl;
        delete_file(user_reservations);
        delete_file(events_reservations);
        close(event_fd);
        return false;
    }

    if(!write_res_file(event_res_file(eid), reserved_seats + people)){
        delete_file(user_reservations);
        delete_file(events_reservations);
        close(event_fd);
        return false;
    }
    close(event_fd);
    return true;
}
