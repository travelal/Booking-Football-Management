#ifndef BOOKING_FUNCTION_H
#define BOOKING_FUNCTION_H
#include "bookingEntity.h"

void bookingMenu();
void addBooking();
void cancelBooking();
void checkAndUpdateFieldStatus(const string& fieldId, const string& date);
void saveBookingToFile();
void loadBookingToFile();

#endif