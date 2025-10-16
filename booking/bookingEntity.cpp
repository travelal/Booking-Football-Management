#include "bookingEntity.h"

Booking::Booking (string bId, string fId, string cName, string pNumber, string d, int iDt, bool s) {
    this-> bookingId = bId;
    this-> fieldId = fId;
    this-> customName = cName;
    this-> phoneNumber = pNumber;
    this-> date = d;
    this-> idTimeSlot = iDt;
    this-> status = s;
}
