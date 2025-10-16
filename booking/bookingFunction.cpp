#include <iostream>
#include <vector>
#include <fstream>
#include "bookingEntity.h"
#include "bookingFunction.h"
#include "../field/fieldFunction.h"
using namespace std;

vector<Booking> bookings;

void bookingMenu(){
    int choice;
    do {
        cout << "\n====== BOOKING MANAGEMENT =======\n";
        cout << "1. Add New Booking\n";
        cout << "2. Cancel Booking\n";
        cout << "3. Display Booking\n";
        cout << "4. Save Booking to File\n";
        cout << "5. Load Booking to File\n";
        cout << "0. Back\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice){
            case 1:
                addBooking();
                break;
            case 2:
                cancelBooking();
                break;
            case 3:
                displayBooking();
                break;
            case 4:
                saveBookingToFile();
                break;
            case 5:
                loadBookingToFile();
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

void addBooking(){
    Booking b;
    cout << "Enter ID: "; cin >> b.bookingId;
    cout << "\n===== AVAILABLE FIELDS =====\n";
    loadFileOfField();
    cout << "Enter your Field ID want to book: "; cin >> b.fieldId;
    cout << "Enter Customer Name: \n"; cin >> b.customName;
    cout << "Enter Phone Number: \n"; cin >> b.phoneNumber;
    cout << "Enter Date (dd-mm-yyyy): \n"; cin >> b.date;
    cout << "Enter Your ID TimeSlot: \n"; cin >> b.idTimeSlot;

    for (auto &field : fields){
        if (field.id == b.fieldId){
            for (auto &timeSlot : field.timeSlots){
                if (timeSlot.id == b.idTimeSlot){
                    timeSlot.isAvailable = false;
                    bool fieldHasAvailableSlot = false;
                    for (const auto &slot : field.timeSlots){
                        if (slot.isAvailable){
                            fieldHasAvailableSlot = true;
                            break;
                        }
                    }
                    field.isAvailable = fieldHasAvailableSlot;
                    saveFileOfField();
                    break;
                    
                }
            }
            break;
        }
    }
    saveBookingToFile();
    bookings.push_back(b); 
    cout << "Booking success !!";
};

void cancelBooking(){
    string id;
    cout << "Enter Booking ID to cancel: \n";
    cin >> id;

    for (auto it = bookings.begin(); it != bookings.end(); ++it){
        if (it->bookingId == id){
            bookings.erase(it);
            cout << "Cancel booking success!!";
            return;
        }
    }
};

void displayBooking(){
    if (bookings.empty()){
        cout << " NO field in display ";
        return;
    }
    for (auto &b: bookings){
        cout << b.bookingId << " - Field " << b.fieldId << " - " 
             << b.customName << " - " << b.phoneNumber << " - " 
             << b.date << " - slot " << b.idTimeSlot << " - " << 
             (b.status ? "booked success" : "Booked fail") << endl;
    }
};

void saveBookingToFile(){
    ofstream file("data/booking.txt");
    for (auto &b : bookings){
        file << b.bookingId << ". Field: "
             << b.fieldId << " |Name: "
             << b.customName << " |Phone Number: "
             << b.phoneNumber << " |Date: "
             << b.date << " |Slot: "
             << b.idTimeSlot << " | "
             << (b.status ? "Success" : "Fail") << endl;
    }
    file.close();
    cout << "Booking saved to file succesfully!! ";
};

void loadBookingToFile(){
    ifstream file ("data/booking.txt");
    bookings.clear();
    if (!file){
        cout << "No Information In File" << endl;
        return;      
    }
    Booking b;
    string line;
    cout << "===== Information Booking =====" << endl;
    while (getline(file, line)){
        cout << line << endl;
    }
    file.close();
}