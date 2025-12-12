#ifndef __RESULTCODES__HPP__
#define __RESULTCODES__HPP__

enum class LoginResult{
    SUCCESS,
    REGISTERED,
    WRONG_PASS,
    IO_ERROR
};

enum class LogoutResult{
    SUCCESS,
    NOT_LOGGED_IN,
    NOT_REGISTERED,
    WRONG_PASS,
    IO_ERROR
};

enum class UnregisterResult{
    SUCCESS,
    NOT_LOGGED_IN,
    NOT_REGISTERED,
    WRONG_PASS,
    IO_ERROR
};

enum class MyEventsResult{
    SUCCESS,
    NO_EVENTS_CREATED,
    NOT_LOGGED_IN,
    WRONG_PASS,
};

#endif