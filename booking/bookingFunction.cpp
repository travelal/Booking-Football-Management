#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "bookingEntity.h"
#include "bookingFunction.h"
#include "../field/fieldFunction.h"
#include "../booking-field/bookingField.h"
using namespace std;

vector<Booking> bookings;

void bookingMenu(){
    int choice;
    do {
        cout << "\n====== BOOKING MANAGEMENT =======\n";
        cout << "1. Add New Booking\n";
        cout << "2. Cancel Booking\n";
        cout << "3. Display All Bookings \n";
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
                loadBookingToFile(); // hiển thị booking nè 
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
    cout << "Enter Booking ID: "; cin >> b.bookingId;
    cout << "Enter Date Booking (dd-mm-yyyy): "; cin >> b.date;
    showAvailableFieldsByDate(b.date);
    
    cout << "Enter your Field ID want to book: "; cin >> b.fieldId;
    if (!isFieldExists(b.fieldId)){
        cout << "Field " << b.fieldId << " does not exist!\n";
        return;
    }

    if (isFieldFullyBooked(b.fieldId, b.date)){
        cout << "\nFIELD " << b.fieldId << " IS FULLY BOOKED ON " << b.date << "!\n";
        cout << "All time slots have been booked for this date.\n";
        
        // CHO NGƯỜI DÙNG 2 LỰA CHỌN
        cout << "\nPlease choose:\n";
        cout << "1. Select another date\n";
        cout << "2. Cancel booking\n";
        cout << "Choose option (1 or 2): ";

        int option;
        cin >> option;

        if (option == 1){
            cout << "Enter new date (dd-mm-yyyy): \n";
            cin >> b.date;
        } else {
            cout << "Booking cancelled!\n";
            return;
        }
    }

    Field selectedField = loadFieldFromFileById(b.fieldId);
    if (selectedField.id.empty()){
        cout << "Field not found!\n";
        return;
    }
    
    // Hiển thị chỉ available slots
    showAvailableSlots(b.fieldId, b.date);
    
    cout << "Enter Your ID TimeSlot: "; cin >> b.idTimeSlot;
    
    // KIỂM TRA QUAN TRỌNG: Slot có tồn tại trong field không
    bool fieldSlotAvailable = false;
    for (auto &slot : selectedField.timeSlots) {
        if (slot.id == b.idTimeSlot) {
            fieldSlotAvailable = true;
            break;
        }
    }
    if (!fieldSlotAvailable){
        cout << "Time slot ID " << b.idTimeSlot << " does not exist for this field! Booking FAILED!\n";
        cout << "Enter Customer Name: "; cin.ignore(); getline(cin, b.customName);
        cout << "Enter Phone Number: "; getline(cin, b.phoneNumber);
        b.status = false;
        bookings.push_back(b);
        saveBookingToFile();
        cout << "Booking saved as FAILED - Invalid time slot!\n";
        return;
    }
    
    // KIỂM TRA QUAN TRỌNG: Slot có thực sự available không
    bool isAvailable = isTimeSlotAvailable(b.fieldId, b.idTimeSlot, b.date);
    if (!isAvailable) {
        cout << "ERROR: Time slot " << b.idTimeSlot << " is already BOOKED! Please choose another slot.\n";
        showAvailableSlots(b.fieldId, b.date);
        
        // KHÔNG cho phép booking failed, mà bắt buộc chọn lại hoặc hủy
        cout << "Do you want to:\n";
        cout << "1. Choose another time slot\n";
        cout << "2. Cancel booking\n";
        cout << "Choose option (1 or 2): ";
        
        int option;
        cin >> option;
        
        if (option == 1) {
            // Gọi lại hàm để chọn slot mới
            addBooking();
            return;
        } else {
            cout << "Booking cancelled.\n";
            return;
        }
    }
    
    // Nếu slot hợp lệ và available, tiến hành booking thành công
    cout << "Enter Customer Name: "; cin.ignore(); getline(cin, b.customName);
    cout << "Enter Phone Number: "; getline(cin, b.phoneNumber);

    b.status = true;
    bookings.push_back(b);
    saveBookingToFile();
    
    BookingField bf;
    bf.bookingId = b.bookingId;
    bf.fieldId = b.fieldId;
    bf.idTimeSlot = b.idTimeSlot;
    bf.date = b.date;
    bf.status = true;

    vector<BookingField> currentBookingFields = loadBookingFieldFromFileInVector();
    bool exists = false;
    for (auto &existingBf : currentBookingFields) {
        if (existingBf.fieldId == bf.fieldId && 
            existingBf.idTimeSlot == bf.idTimeSlot && 
            existingBf.date == bf.date) {
            exists = true;
            break;
        }
    }
    
    if (!exists) {
        currentBookingFields.push_back(bf);
        // Lưu lại toàn bộ danh sách
        ofstream file("data/bookingField.txt");
        if (file.is_open()) {
            for (auto &item : currentBookingFields) {
                file << "Field " << item.fieldId << "| " 
                     << item.idTimeSlot << "| " 
                     << item.date << "| " 
                     << item.bookingId << "| " 
                     << (item.status ? "Booked" : "Available") << endl;
            }
            file.close();
            cout << "BookingField data saved successfully!\n";
        }
    }

    cout << "Booking success !!\n";
    
    // Kiểm tra nếu field đã fully booked
    checkAndUpdateFieldStatus(b.fieldId, b.date);
};

void checkAndUpdateFieldStatus(const string& fieldId, const string& date){
    if (isFieldFullyBooked(fieldId, date)){
        cout << "Notice: Field " << fieldId 
             << " is fully booked on " << date 
             << "!\n";
    }
}
void cancelBooking() {
    string id;
    cout << "Enter Booking ID to cancel: ";
    cin >> id;

    // Tìm booking theo ID
    auto it = find_if(bookings.begin(), bookings.end(), 
        [&id](const Booking& b) { return b.bookingId == id; });
    
    if (it == bookings.end()) {
        cout << " Booking ID " << id << " not found!\n";
        return;
    }

    // Kiểm tra booking status
    if (!it->status) {
        cout << " This booking is already FAILED, cannot cancel!\n";
        return;
    }
    
    string fieldId = it->fieldId;
    string date = it->date;
    int timeSlotId = it->idTimeSlot;
    // Xóa khỏi danh sách bookings
    bookings.erase(it);
    
    // Xóa khỏi bookingFields
    vector<BookingField> bfList = loadBookingFieldFromFileInVector();
    auto bfIt = find_if(bfList.begin(), bfList.end(),
        [&id](const BookingField& bf) { return bf.bookingId == id; });
    
    if (bfIt != bfList.end()) {
        bfList.erase(bfIt);
    }
    
    // Lưu bookingFields
    ofstream bfFile("data/bookingField.txt");
    if (bfFile.is_open()) {
        for (auto &bf : bfList) {
            bfFile << "Field " << bf.fieldId << "| " 
                 << bf.idTimeSlot << "| " 
                 << bf.date << "| " 
                 << (bf.status ? "Booked" : "Available") << endl;
        }
        bfFile.close();
    }
    
    // Lưu bookings
    saveBookingToFile();
    
    cout << "Cancel booking SUCCESS! Time slot has been freed.\n";

    if (isFieldFullyBooked(fieldId, date)){
        cout << "Notice: Field " << fieldId 
             << " now has available slots for " << date 
             << "!\n";
    }
}

void saveBookingToFile(){
    ofstream file("data/booking.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open file for writing!" << endl;
        return;
    }
    
    for (const Booking &b : bookings) {
        file << b.bookingId << " | "
             << b.fieldId << " | "
             << b.customName << " | "
             << b.phoneNumber << " | "
             << b.date << " | "
             << b.idTimeSlot << " | "
             << (b.status ? "Success" : "Failed")
             << "\n";
    }
    file.close();
    cout << "Booking saved to file successfully!! ";
}

void loadBookingToFile() {
    ifstream file("data/booking.txt");
    bookings.clear();
    if (!file.is_open()) {
        cout << "No booking file found or cannot open file!" << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        Booking b;
        stringstream ss(line);
        string token;
        bool isValid = true;

        vector<string> parts;
        while (getline(ss, token, '|')){
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            parts.push_back(token);
        }
        
        if (parts.size() != 7) continue;
        
        b.bookingId = parts[0];
        b.fieldId = parts[1];
        b.customName = parts[2];
        b.phoneNumber = parts[3];
        b.date = parts[4];
        b.status = (parts[6] == "Success");
        
        try {
            b.idTimeSlot = stoi(parts[5]);
        } catch (const std::invalid_argument& e) { 
            isValid = false;
            continue;
        }

        b.status = (parts[6] == "Success");
        bookings.push_back(b);
    }
    file.close();

    cout << "\n=== All BOOKINGS ===" << endl;
    if (bookings.empty()){
        cout << "No Booking found!";
    }else{
        for (auto &b : bookings) {
            // Trong loadBookingToFile() - sửa phần hiển thị
            cout << "ID: " << b.bookingId 
                 << " | Field: " << b.fieldId 
                 << " | Customer: " << b.customName 
                 << " | Phone: " << b.phoneNumber
                 << " | Date: " << b.date 
                 << " | Slot: " << b.idTimeSlot 
                 << " | Status: " << (b.status ? "Success" : "Failed") << endl;
        }
    }
}