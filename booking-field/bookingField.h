#ifndef BOOKING_FIELD_H
#define BOOKING_FIELD_H

#include <vector>
#include <string>
#include "../field/fieldEntity.h"
#include "../booking/bookingEntity.h"
using namespace std;

class BookingField{
    public:
    string bookingId;
    string fieldId;
    int idTimeSlot;
    string date;
    bool status;    
};
extern vector<BookingField> bookingFields;

void saveBookingFieldToFile();
bool isTimeSlotAvailable(string fieldId, int idTimeSlot, const string &date);
bool isFieldFullyBooked(string fieldId, const string &date);
bool isFieldExists(string fieldId);
void showAvailableFieldsByDate(const string &date);
void showAvailableSlots(string fieldId, const string &date);
vector<BookingField> loadBookingFieldFromFileInVector();
Field loadFieldFromFileById(string fieldId);
#endif