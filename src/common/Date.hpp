#ifndef __DATE__HPP__
#define __DATE__HPP__

class Date {
private:
    int day;
    int month;
    int year;

    // Returns how many days are on a month.
    int daysInMonth(int m, int y);
    // Checks if date is valid.
    bool isValidDate(int d, int m, int y);

public:
    // Constructor
    Date(int d = 1, int m = 1, int y = 2000);

    // Getters
    int getDay();
    int getMonth();
    int getYear();

    // Setters
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    // Methods

    // Checks if date is invalid from the constructor.
    bool invalidDate();
    // Advances the date by one day
    void nextDay();  
    // Prints the date in DD/MM/YYYY format
    void print();    
    // Compares this date to another
    bool isAfter(Date& other);
    // Checkls if date is before current date.
    bool isPast();
};

#endif
