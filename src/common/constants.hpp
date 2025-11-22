#ifndef __CONSTANTS__HPP__
#define __CONSTANTS__HPP__

#define DEFAULT_IP "127.0.0.1" // Localhost
#define DEFAULT_PORT "58009"   // 58000 + 9(Group Number)

#define UDP_TIMEOUT 2  // 2 seconds
#define UDP_RETRIES 3  

#define UID_SIZE 7         //  6 + 1 for '\0'
#define PASS_SIZE 9        //  8 + 1 for '\0'
#define EVENT_NAME_SIZE 11 // 10 + 1 for '\0'
#define EID_SIZE 4         //  3 + 1 for '\0'
#define FILE_NAME_SIZE 25  // 24 + 1 for '\0'

#define BUFFER_SIZE 256
#define BUF_TEMP 64  

#endif