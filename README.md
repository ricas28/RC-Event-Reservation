# 🎉 Event Reservation System

**Course:** Computer Networks – 1st Semester 2025/2026  
**Group:** 9  
**Authors:**  
- Ricardo Santos (109339)  
- Filipe Neves (110726)  

**Language:** C++  
**Platform:** Linux  

---

## 📑 Table of Contents
1. [General Description](#-general-description)
2. [Project Structure](#-project-structure)
3. [Database Structure](#-database-structure)
4. [Compilation](#-compilation)
5. [Execution](#-execution)
6. [Supported Commands](#-supported-commands)
7. [Implementation Decisions](#-implementation-decisions)
8. [Final Notes](#-final-notes)

---

## 🎯 General Description

This project implements an **Event Reservation System** with a **client-server architecture**.

**Components:**
- **🖥 Event Server (ES):** Manages users, events, and reservations.  
- **📱 User Application (User):** Lets users login, create/close events, reserve spots, and list events.

**Communication Model:**
- **📡 UDP:** Lightweight, stateless operations (login, logout, unregister, list events/reservations).  
- **🔗 TCP:** Operations involving state changes or file transfers (create/close/show/reserve events, file uploads, password change).  

All communications are multiplexed using `select()` for efficiency.

---

## 🗂 Project Structure

src/
├── common/ # Shared utilities, constants, protocol handling, date/time logic
├── server/ # Server implementation, database management, TCP/UDP handlers
└── client/ # Client-side application and protocol implementation

**Separation of concerns:**
- Protocol logic  
- Database handling  
- Network communication  
- Application logic  

---

## 💾 Database Structure

**Root directory:** `./ES_DB/`

### Users

The database is filesystem-based and stored under the root directory:
./ES_DB/

### Users
USERS/
└── <UID>/
├── <UID>_pass.txt # User password
├── <UID>_login.txt # Login status
├── CREATED/ # Event IDs created by the user
└── RESERVED/ # Reservation files for reserved events

### Events
EVENTS/
└── <EID>/
├── START_<EID>.txt # Event metadata (creator, name, date, capacity, etc.)
├── RES_<EID>.txt # Number of reserved seats
├── DESCRIPTION/ # Uploaded description files
├── RESERVATIONS/ # Reservation files per user
└── END_<EID>.txt # Created when the event is closed

### Global Files
EID_COUNTER.txt # Stores the last assigned event ID


**Design Notes:**
- ✅ Files are locked before reading/writing to prevent concurrent corruption.  
- ✅ Directories are created on demand (`CREATED`, `RESERVED`, `DESCRIPTION`, `RESERVATIONS`).  
- ✅ Events are listed **numerically by EID**.  
- ⚠️ Reservation files store the **EID instead of UID**, unlike the original specification.  
- Maximum events (`MAX_EVENTS`) and reservations per user (`MAX_RESERVATIONS_SEND`) are enforced.

---

## 🛠 Compilation

From the project root:

```bash
make
```
This compiles both the server and the client executables.

--- 

## 🚀 Execution
Start the Event Server
```bash
./ES [-p ESport] [-v]
```

```bash
Start the User Application
./user [-n ESIP] [-p ESport]
```

Optional Arguments:
- `ESIP`: 🌐 Server IP (default: localhost)
- `ESport`: 🔢 Server port (default: 58000 + group number)
- `-v`: 🗯 Verbose mode (server only)

---

## 📜 Supported Commands

Here's a neat overview of the commands you can use in the **User** application:

| Command | Description |
|---------|-------------|
| `login UID password` | Log in with your user ID and password. |
| `changePass oldPassword newPassword` | Change your current password. |
| `logout` | Log out from the server. |
| `unregister` | Delete your user account. |
| `create name event_file event_date num_attendees` | Create a new event with a description file. |
| `close EID` | Close an event you created. |
| `list` | List all available events. |
| `show EID` | Show detailed information about a specific event. |
| `reserve EID value` | Reserve a number of seats for an event. |
| `myevents` / `mye` | List all events you have created. |
| `myreservations` / `myr` | List all events you have reserved. |
| `exit` | Exit the client application. |

💡 **Tip:** Use the short forms `mye` and `myr` to save some typing!  
🎯 Commands are designed to work seamlessly with both TCP (for state-changing actions) and UDP (for quick queries).  

---

## 🧠 Implementation Decisions

- **Multiplexing:** UDP + TCP handled with `select()`.  
- **Concurrency:** Each TCP request in a forked child process.  
- **Timeouts:** TCP connections use `SO_RCVTIMEO` to avoid blocking.  
- **Signals:**  
  - `SIGPIPE` ignored to prevent crashes.  
  - `SIGCHLD` ignored to avoid zombies.  
  - `SIGINT` triggers clean shutdown.  
- **Robustness:** Defensive programming to handle wrong protocol messages or system call errors.  
- **Error responses:** `"NOK\n"` sent if server cannot fulfill request.  
- **File transfers:** Exact size is read from TCP stream and written to disk; file integrity verified externally.  
- **Events ordering:** Sorted numerically by EID.  
- **Event corruption:** Marked only if data exists but is inconsistent.  
- **Database initialization:** Ensures directories exist before server starts.  
- **Server startup:** Prints IPv4 address and port for debugging.

---

## 🏁 Final Notes

This project emphasizes:
- ✅ Correctness  
- ✅ Robustness  
- ✅ Proper client-server communication  
- ✅ Safe concurrency  
- ✅ Defensive error handling  

Tested in local and remote environments.

✨ Enjoy reserving your events! 🎫

