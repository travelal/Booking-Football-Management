#include "bookingField.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../field/fieldFunction.h"
#include "../booking/bookingFunction.h"
using namespace std;

extern vector<Field> fields;
extern vector<Booking> bookings;
vector<BookingField> bookingFields;

void saveBookingFieldToFile(){
    ofstream file("data/bookingField.txt");
    if (!file.is_open()) return;
    
    for (auto &bf :bookingFields){
        file <<"Field "<< bf.fieldId << "| " 
             << bf.idTimeSlot << "| " 
             << bf.date << "| " << bf.bookingId << "| "
             << (bf.status ? "Booked" : "Available") << endl;
    }
    file.close();
    cout << "BookingField data saved successfully!\n";
};

bool isTimeSlotAvailable(string fieldId, int timeSlotId, const string &date) {
    // Load booking fields từ file để có dữ liệu mới nhất
    vector<BookingField> currentBookings = loadBookingFieldFromFileInVector();
    
    for (auto &bf : currentBookings){
        if (bf.fieldId == fieldId && bf.idTimeSlot == timeSlotId && bf.date == date && bf.status) {
            return false;
        }
    }
    return true;
}

bool isFieldFullyBooked(string fieldId, const string & date){
    Field f = loadFieldFromFileById(fieldId);
    if (f.id.empty()) return false;

    for (auto &slot : f.timeSlots){
        if (isTimeSlotAvailable(fieldId, slot.id, date)){
            return false;
        }
    }
    return true;
}

void showAvailableSlots(string fieldId, const string &date){
    Field f = loadFieldFromFileById(fieldId);
    if (f.id.empty()){
        cout << "Field not found !\n";
        return;
    };
    
    if (isFieldFullyBooked(fieldId, date)) {
        cout << "\n FIELD " << fieldId << " IS FULLY BOOKED ON " << date << "!\n";
        cout << "All time slots have been booked for this date.\n";
        return;
    }

    bool foundAvailable = false;
    bool foundBooked = false;
    cout << "\n=== AVAILABLE SLOTS FOR FIELD " << fieldId << " ON " << date << " ===" << endl;

    for (auto &slot : f.timeSlots){
        bool isAvailable = isTimeSlotAvailable(fieldId, slot.id, date);
        
        if (isAvailable){
            cout << " Slot ID: " << slot.id << " [" << slot.startTime << "h" 
                 << (slot.startMin < 10 ? "0" : "") << slot.startMin << "p - " 
                 << slot.endTime << "h" << (slot.endMin < 10 ? "0" : "") << slot.endMin << "p]\n";
            foundAvailable = true;
        } else {
            cout << " Slot ID: " << slot.id << " [" << slot.startTime << "h" 
                 << (slot.startMin < 10 ? "0" : "") << slot.startMin << "p - " 
                 << slot.endTime << "h" << (slot.endMin < 10 ? "0" : "") << slot.endMin << "p] - BOOKED\n";
            foundBooked = true;
        }
    }

    if (!foundAvailable){
        cout << "No available slots found for this field on this date.\n";
    }
    cout << "==========================================" << endl;
}

void showAvailableFieldsByDate(const string &date) {
    cout << "\n=== AVAILABLE FIELDS AND SLOTS FOR " << date << " ===" << endl;
    bool foundAnyAvailableSlot = false;
        
    vector<Field> fields = loadFieldFromFileInVectorType();
    for (auto &field : fields) {
        bool fieldHasAvailableSlots = false;
        int availableSlotCount = 0;
        
        // Kiểm tra nhanh field có slot available không
        for (auto &slot : field.timeSlots) {
            if (isTimeSlotAvailable(field.id, slot.id, date)) {
                fieldHasAvailableSlots = true;
                foundAnyAvailableSlot = true;
                availableSlotCount++;
            }
        }
        
        if (!fieldHasAvailableSlots) {
            cout << "\n Field: " << field.name << " (ID: " << field.id << ") - FULLY BOOKED" << endl;
            continue;
        }
        
        cout << "\n Field: " << field.name << " (ID: " << field.id << ")" << endl;
        cout << " Location: " << field.location << " | Rate: " << field.hourlyRate << "$/h" << endl;
        cout << "Available slots: " << availableSlotCount << "/" << field.timeSlots.size() << endl;
        cout << "----------------------------------------" << endl;

        for (auto &slot : field.timeSlots) {
            bool isAvailable = isTimeSlotAvailable(field.id, slot.id, date);
            
            string startTimeStr = to_string(slot.startTime) + "h" + 
                                 (slot.startMin < 10 ? "0" : "") + to_string(slot.startMin) + "p";
            string endTimeStr = to_string(slot.endTime) + "h" + 
                               (slot.endMin < 10 ? "0" : "") + to_string(slot.endMin) + "p";
            
            if (isAvailable) {
                cout << "   Slot " << slot.id << ": " << startTimeStr << " - " << endTimeStr << endl;
            } else {
                cout << "   Slot " << slot.id << ": " << startTimeStr << " - " << endTimeStr << " - [BOOKED]" << endl;
            }
        }
            
        if (availableSlotCount > 0) {
            cout << "Total available slots: " << availableSlotCount << endl;
            foundAnyAvailableSlot = true;
        }
        cout << endl;
    }
        
    if (!foundAnyAvailableSlot) {
        cout << "\n  NO AVAILABLE SLOTS FOUND for " << date << endl;
        cout << "Please select another date.\n";
    }
    cout << "==================================================" << endl;
};

vector<BookingField> loadBookingFieldFromFileInVector(){
    vector<BookingField> loadedBookings;
    ifstream file("data/bookingField.txt");
    if (!file.is_open()){
        cout << "Cannot open bookingField.txt file!" << endl;
        return loadedBookings;
    };

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        BookingField bf;
        stringstream ss(line);
        vector<string> parts;
        string token;
        bool isValid = true;

        // Parse theo format: "Field 1| 1| 20-10-2025| Booked"
        while (getline(ss, token, '|')) {
            // Xóa khoảng trắng
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            parts.push_back(token);
        }
        
        if (parts.size() < 4) {
            continue;
        }
        
        // Parse fieldId: "Field 1" -> "1"
        if (parts[0].find("Field ") == 0) {
            bf.fieldId = parts[0].substr(6); // Bỏ "Field " (6 ký tự)
        } else {
            bf.fieldId = parts[0];
        }
        
        try {
            bf.idTimeSlot = stoi(parts[1]);
        } catch (const std::invalid_argument& e) {
            cout << " Invalid timeslot: " << parts[1] << endl;
            isValid = false;
        }
        
        bf.date = parts[2];

        if (parts.size() >= 5) {
            bf.bookingId = parts[3]; // bookingId nằm ở vị trí mới
            bf.status = (parts[4] == "Booked");
        }else{
            bf.bookingId = "";
            bf.status = (parts[3] == "Booked");
        }
        
        if (isValid){
            loadedBookings.push_back(bf);
        }
    }
    file.close();
    return loadedBookings;
}

bool isFieldExists(string fieldId){
    vector<Field> fields = loadFieldFromFileInVectorType();
    for (auto &field : fields){
        if (field.id == fieldId){
            return true;
        }
    }
    return false;
}