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

enum class MyReservationsResult{
    SUCCESS,
    NO_RESERVATIONS_MADE,
    NOT_LOGGED_IN,
    WRONG_PASS
};

enum class CreateResult{
    SUCCESS,
    FAILED_CREATE,
    NOT_LOGGED_IN,
    WRONG_PASS,
    IO_ERROR
};

enum class CloseResult{
    SUCCESS,
    WRONG_USER_PASS,
    NOT_LOGGED_IN,
    EVENT_DOES_NOT_EXIST,
    NOT_CREATED_BY_USER,
    SOLD_OUT,
    ALREADY_PASSED,
    ALREADY_CLOSED,
    IO_ERROR,
};

#endif