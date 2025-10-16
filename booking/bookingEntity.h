#ifndef BOOKING_ENTITY_H
#define BOOKING_ENTITY_H
#include <string>
#include <vector>
using namespace std;

class Booking {
    public:
        string bookingId;
        string fieldId;
        string customName;
        string phoneNumber;
        string date;
        int idTimeSlot;
        bool status;

    Booking( string bId = "", string fId ="", string cName= "", string pNumber="", string d="", int iDt=0, bool s= true);
};

#endif