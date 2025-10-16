#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "fieldEntity.h"
#include "fieldFunction.h"
using namespace std;

vector<Field> fields; //mảng động field , hay còn gọi là danh sách, kieu du lieu chung

void fieldMenu(){
    int choose;
    do{
        cout << "\n--- Field Management ---\n";
        cout << "1. Add Field\n";
        cout << "2. Edit Field\n";
        cout << "3. Delete Field\n";
        cout << "4. Display Fields\n";
        cout << "5. Find Fields\n";
        cout << "6. Save Fields to File\n";
        cout << "7. Load Fields from File\n";
        cout << "0. Back\n";
        cout << "Choose: ";
        cin>>choose;
        switch (choose){
            case 1: 
                addField();
                break;
            case 2: 
                editField();
                break;
            case 3: 
                deleteField();
                break;
            case 4: 
                displayField();
                break;
            case 5:
                findField();
                break;            
            case 6: 
                saveFileOfField();
                break;
            case 7: 
                loadFileOfField();
                break;
            case 0: 
            break;
            default: cout<<"invalid choice";
        }
    }while ( choose != 0 );
};

void addField() {
    Field f;
    cout << "Enter ID: "; cin >> f.id;
    cout << "Enter Name: "; cin.ignore(); getline(cin, f.name);
    cout << "Enter Location: "; getline(cin, f.location);
    cout << "Enter Size (number of players): "; cin >> f.size;
    cout << "Enter Hourly Rate: "; cin >> f.hourlyRate;
    
    int idTimeSlot;
    cout << "How many time slots does this field have? ";
    cin >> idTimeSlot;
    
    for (int i = 0; i < idTimeSlot ; i++) {
        time_t startTime, endTime;
        int startMin, endMin;
        cout << "Slot " << i + 1 << ": Enter start time (Hour Min)): ";
        cin >> startTime >> startMin;
        cout << "Slot " << i + 1 << ": Enter end time (Hour Min)): ";
        cin >> endTime >> endMin;
        f.timeSlots.push_back(TimeSlot(i + 1, startTime, endTime, startMin, endMin, true));
    }

    f.isAvailable = true;
    fields.push_back(f);
    cout << "Field added sucessfully! ";
    saveFileOfField();
};

void editField() {
    string id;
    Field f;
    cout << "Enter Field ID to edit: ";
    cin >> id;
    for (auto &f : fields) {
        if (f.id == id) {
            int choice;
                do {
                    cout << "--- Edit Field ---\n";
                    cout << "1. Edit basic info field (name, location, size, rate)\n";
                    cout << "2. Edit field status\n";
                    cout << "3. Edit time slots\n";
                    cout << "4. Edit time slots status\n";
                    cout << "0. Back\n";
                    cout << "Choice One: ";
                    cin >> choice;
                    switch ( choice ) {
                        case 1:
                            editBasicField(id);
                            break;
                        case 2:
                            editFieldStatus(id);
                            break;
                        case 3:
                            editTimeSlots();
                            break;
                        case 4:
                            editTimeSlotStatus();
                            break;
                    }
                } while (choice !=0);
        }
    }saveFileOfField();
};

void editBasicField(string id){
    for (auto &f : fields){
        if ( f.id == id ){
            cout << "Enter new name: "; cin.ignore(); getline(cin, f.name);
            cout << "Enter new location: "; getline(cin, f.location);
            cout << "Enter new size: "; cin >> f.size;
            cout << "Enter new hourly rate: "; cin >> f.hourlyRate;
            return;
        }
    }saveFileOfField();
    cout << "Field with ID" << id << "not found" << endl;        
};

void editFieldStatus(string id){
    for (auto &f : fields){
        if ( f.id == id ){
            int status;
            cout << "Current status" << (f.isAvailable ? "Available" : "Booked") << endl;
            cout << "Set field status ( 1. Available, 2. Booked )";
            cin >> status;
            if (status == 1) {
                f.isAvailable = true;
                cout << "--- Status updated to Available! --- \n";
            } else if (status == 2) {
                f.isAvailable = false;
                cout << "--- Status updated to Booked! --- \n";
            } else {
                cout << "--- Invalid choice! Please enter 1 or 2. --- \n";
            }
            return;
        }
    }saveFileOfField();
};

void updateFieldStatus(Field & field) {
    if (field.timeSlots.empty()){
        field.isAvailable = true;
        return;
    }
    
    bool allBooked = true;
    for (auto &timeSlot : field.timeSlots){
        if (timeSlot.isAvailable) {
            allBooked = false;
            break;
        }
    }
    field.isAvailable = !allBooked;
    if (allBooked){
        cout << "--- Field " << field.id << " status updated to Booked (all time slots booked)! ---" << endl;
    }saveFileOfField();
};

void editTimeSlots(){
    string fieldId;
    cout << "Enter field ID to edit time slot :";
    cin >> fieldId;
    for (auto &f : fields){
        if (f.id == fieldId){
            int slotID;
            cout << "Enter Time Slot ID: ";
            cin >> slotID;

            for (auto &timeSlot : f.timeSlots){
                if (timeSlot.id == slotID){
                    cout << "Editing Time Slot: " << slotID << endl;
                    cout << "Current: " << timeSlot.startTime << "h"<< timeSlot.startMin << "p" << " - "
                         << timeSlot.endTime << "h" << timeSlot.endMin <<"p" <<" - "
                         <<(timeSlot.isAvailable ? "Available" : "Booked") <<endl;
                    
                    cout << "Enter new start time (hour min): ";
                    cin >> timeSlot.startTime >> timeSlot.startMin;
                    cout << "Enter new end time (hour min): ";
                    cin >> timeSlot.endTime >> timeSlot.endMin;
                    cout << " ===Change Time Slot Success !!!=== ";
                }
            }
        }
    }saveFileOfField();
};

void editTimeSlotStatus(){
    string fieldId;
    cout << "Enter field ID to edit: ";
    cin >> fieldId;
    for (auto &f : fields){
        if (f.id == fieldId){
            int slotId;
            cout << "Enter ID time slot field: ";
            cin >> slotId;

            for (auto &timeSlot : f.timeSlots){
                if (timeSlot.id == slotId){
                    cout << "Editing time status" << slotId <<endl;
                    cout << "Current: " << timeSlot.startTime << "h " << timeSlot.startMin << "p - "
                    << timeSlot.endTime << "h" << timeSlot.endMin << "p - "
                    << (timeSlot.isAvailable ? "Available" : "Booked") << endl;

                    cout << "Set time slot status (1. IsAvailable, 2. Booked): ";
                    int statusTimeSlot;
                    cin >> statusTimeSlot;
                    if (statusTimeSlot == 1) {
                        timeSlot.isAvailable = true;
                        cout << "--- Time Slot Status update IsAvailable---\n";
                    }else if (statusTimeSlot == 2){
                        timeSlot.isAvailable = false;
                        cout << "--- Time Slot Status update Booked ---\n";
                    }else {
                        cout << "--- Invalid choice! Please choice 1 or 2 ---\n";
                    }
                    updateFieldStatus(f);
                    return;
                }
            }
        }
    }saveFileOfField();
};

void displayField(){
    if (fields.empty()){
        cout << "No Field" << endl;
        return;
    }
    
    for( auto &f : fields){
        cout <<f.id<< " - " << f.name << "-" << f.location
             << " -  " << f.size << " - " <<f.hourlyRate <<"$/h"
             << " - " << (f.isAvailable ? "Available" : "Booked") << endl;

        
        cout << " Time Slots: " << endl;
        if (f.timeSlots.empty()){
            cout << "No time available" << endl;
        } else {
            int i;
            for (i=0; i < f.timeSlots.size(); i++){
                cout << f.timeSlots[i].id << "." << f.timeSlots[i].startTime <<"h"
                    <<f.timeSlots[i].startMin <<"p"<< " - " << f.timeSlots[i].endTime 
                    << "h" <<f.timeSlots[i].endMin<< "p" <<"-" 
                    << (f.timeSlots[i].isAvailable ? "Available" : "Booked") << endl;
            }
        }
    }
};

void displayAvailableField(){
    if (fields.empty()){
        cout << "No Field" << endl;
        return;
    }
    cout << "\n--- Fields with Available Time Slot---\n";
    bool hasAvailableField = false;

    for (auto &f : fields){
        bool hasAvailableSlot = false;
        for ( auto &f : fields){
            // Check if field has at least one available time slot
            bool hasAvailableSlot = false;
            for (const auto &slot : f.timeSlots){
                if (slot.isAvailable){
                    hasAvailableSlot = true;
                    break;
                }
            }
            
            // Only display fields that have available time slots
            if (hasAvailableSlot){
                hasAvailableField = true;
                cout <<f.id<< " - " << f.name << " - " << f.location
                      << " - " << f.size << " - " <<f.hourlyRate <<"$/h"
                      << " - " << (f.isAvailable ? "Available" : "Booked") << endl;
    
                
                cout << " Available Time Slots: " << endl;
                if (f.timeSlots.empty()){
                    cout << "No time available" << endl;
                } else {
                    int i;
                    for (i=0; i < f.timeSlots.size(); i++){
                        if (f.timeSlots[i].isAvailable){  // Only show available slots
                            cout << "  " << f.timeSlots[i].id << ". " << f.timeSlots[i].startTime <<"h"
                                <<f.timeSlots[i].startMin <<"p"<< " - " << f.timeSlots[i].endTime 
                                << "h" <<f.timeSlots[i].endMin<< "p" << " - " 
                                << (f.timeSlots[i].isAvailable ? "Available" : "Booked") << endl;
                        }
                    }
                }
                cout << endl;
            }
        }
        
        if (!hasAvailableField){
            cout << "No fields with available time slots!" << endl;
        }
    }
}

void deleteField(){
    string id;
    Field f;
    cout << "Enter Field ID to edit: ";
    cin >> id;
    for (auto &f : fields) {
        if (f.id == id) {
            int choice;
                do{
                    cout << "=== DELETE FIELD === \n";
                    cout << "1. Delete Field \n";
                    cout << "2. Delete TimeSLot \n";
                    cout << "0. Exit\n";
                    cin >> choice;

                    switch ( choice ){
                        case 1: 
                            deleteFieldBasic();
                            break;

                        case 2:
                            deleteTimeSlots();
                            break;
                    }
                }while (choice != 0 );                
                cout << "Choice: ";
            }
        }
};
void deleteFieldBasic(){
    string id;
    cout << "Enter Field ID to delete: ";
    cin >> id;

    for(int i = 0; i < fields.size(); i++){
        if (fields[i].id == id) {
            fields.erase(fields.begin() + i );
            cout << "Field delete successfully\n";
            return;
        }
    }
    saveFileOfField();
    cout << "Field not found!" << endl;
};

void deleteTimeSlots(){
    string fieldId;
    cout << "Enter field ID to edit: ";
    cin >> fieldId;
    for (auto &f : fields){
        if (f.id == fieldId){
            int slotId;
            cout << "Enter ID time slot field to delete: ";
            cin >> slotId;

            for (int i = 0; i < f.timeSlots.size(); i++){
                if (f.timeSlots[i].id == slotId){
                    f.timeSlots.erase(f.timeSlots.begin() + i );
                    cout << "--- Time Slot was deleted ! ---\n";
                }
            }
        }
        cout << "=== Time Slot not found ! ===";
        return;
        saveFileOfField();
    }
    cout << "=== Field Not Found ===\n";
};

void findField(){
    string fieldId;
    cout << "Enter field ID to find: ";
    cin >> fieldId;

    for (auto &f : fields){
        if (f.id == fieldId){
            cout << "=== Find Field ===\n";
            cout << f.id << " - " << f.name << " - " 
            << f.location << " - " << f.size << "m - " 
            << f.hourlyRate << "$/h" << " - "
            << (f.isAvailable ? "Available" : "Booked") << endl;
        }
    }
};

void saveFileOfField(){
    ofstream file("data/field.txt");
    for (auto &f : fields){
        file << "Field" << "." << f.id << "." << " "<< f.name << " - " << f.location << " - "
             << f.size << " - " << f.hourlyRate << "$/h - " << (f.isAvailable ? "IsAvailable" : "Booked")<< "\n";
        file << f.timeSlots.size() << " slot:" <<"\n";
        for (const TimeSlot& timeSlot : f.timeSlots){
            file << timeSlot.id << ". " << timeSlot.startTime << "h" 
                 << timeSlot.startMin << "p "<< "-" << timeSlot.endTime << "h" 
                 << timeSlot.endMin << "p - " << (timeSlot.isAvailable ? "IsAvailable" : "Booked") << "\n";

        }
        file << "\n";    
    } 
    file.close();
};

void loadFileOfField(){
    ifstream file("data/field.txt");
    fields.clear();
    if (!file) {
        cout << "No File Information " << endl;
        return;   
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
};