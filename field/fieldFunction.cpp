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

void saveFileOfField() {
    ofstream file("data/field.txt");
    if (!file) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    for (auto &f : fields) {
        file << "Field." << f.id << ". "
             << f.name << " - " << f.location << " - "
             << f.size << " - " << f.hourlyRate << "$/h - "
             << (f.isAvailable ? "IsAvailable" : "Booked") << "\n";

        file << f.timeSlots.size() << " slot:\n";

        for (const TimeSlot &timeSlot : f.timeSlots) {
            // format phút có 2 chữ số nếu < 10
            auto formatMinute = [](int m) {
                string s = to_string(m);
                if (m < 10) s = "0" + s;
                return s;
            };

            file << timeSlot.id << ". "
                 << timeSlot.startTime << "h" << formatMinute(timeSlot.startMin) << "p"
                 << " - "
                 << timeSlot.endTime << "h" << formatMinute(timeSlot.endMin) << "p"
                 << " - "
                 << (timeSlot.isAvailable ? "IsAvailable" : "Booked")
                 << "\n";
        }

        file << "\n";
    }

    file.close();
};

int parseTimeHour(const string& timeStr);
int parseTimeMinute(const string& timeStr);

int parseTimeHour(const string& timeStr) {
    size_t hPos = timeStr.find('h');
    if (hPos != string::npos && hPos > 0) {
        string hourStr;
        for (size_t i = 0; i < hPos; i++) {
            if (isdigit(timeStr[i])) hourStr += timeStr[i];
        }
        return hourStr.empty() ? 0 : stoi(hourStr);
    }
    return 0;
}

int parseTimeMinute(const string& timeStr) {
    size_t hPos = timeStr.find('h');
    size_t pPos = timeStr.find('p');
    if (hPos != string::npos && pPos != string::npos && pPos > hPos) {
        string minStr;
        for (size_t i = hPos + 1; i < pPos; i++) {
            if (isdigit(timeStr[i])) minStr += timeStr[i];
        }
        return minStr.empty() ? 0 : stoi(minStr);
    }
    return 0;
}

void loadFileOfField() {
    ifstream file("data/field.txt");
    fields.clear();
    if (!file) {
        cout << "No File Information " << endl;
        return;
    }

    cout << "=== FIELD DATA IN FILE ===" << endl;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        cout << line << endl;

        // Parse Field line: Field.id. name - location - size - rate$/h - status
        if (line.find("Field.") == 0) {
            Field field;

            size_t pos1 = line.find('.');
            size_t pos2 = line.find('.', pos1 + 1);
            if (pos1 != string::npos && pos2 != string::npos) {
                field.id = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string fieldInfo = line.substr(pos2 + 1);

                size_t dash1 = fieldInfo.find(" - ");
                size_t dash2 = fieldInfo.find(" - ", dash1 + 3);
                size_t dash3 = fieldInfo.find(" - ", dash2 + 3);
                size_t dash4 = fieldInfo.find("$/h - ", dash3 + 3);

                if (dash1 != string::npos && dash2 != string::npos &&
                    dash3 != string::npos && dash4 != string::npos) {

                    field.name = fieldInfo.substr(0, dash1);
                    field.location = fieldInfo.substr(dash1 + 3, dash2 - dash1 - 3);
                    string sizeStr = fieldInfo.substr(dash2 + 3, dash3 - dash2 - 3);
                    string rateStr = fieldInfo.substr(dash3 + 3, dash4 - dash3 - 3);
                    string statusStr = fieldInfo.substr(dash4 + 6);

                    field.size = (sizeStr.empty()) ? 0.0f : stof(sizeStr);
                    field.hourlyRate = (rateStr.empty()) ? 0.0f : stof(rateStr);
                    field.isAvailable = (statusStr == "IsAvailable");

                    // Read time slots
                    if (getline(file, line)) {
                        cout << line << endl;
                        size_t slotPos = line.find(" slot:");
                        if (slotPos != string::npos) {
                            int numSlots = stoi(line.substr(0, slotPos));
                            for (int i = 0; i < numSlots; i++) {
                                if (getline(file, line)) {
                                    cout << line << endl;

                                    size_t dotPos = line.find('.');
                                    if (dotPos != string::npos) {
                                        TimeSlot timeSlot;
                                        timeSlot.id = stoi(line.substr(0, dotPos));

                                        string timePart = line.substr(dotPos + 2);
                                        size_t dashPos = timePart.find(" - ");
                                        size_t lastDash = timePart.rfind(" - ");
                                        if (dashPos != string::npos && lastDash != string::npos) {
                                            string startTimeStr = timePart.substr(0, dashPos);
                                            string endTimeStr = timePart.substr(dashPos + 3, lastDash - (dashPos + 3));
                                            string status = timePart.substr(lastDash + 3);

                                            // Parse times
                                            timeSlot.startTime = parseTimeHour(startTimeStr);
                                            timeSlot.startMin = parseTimeMinute(startTimeStr);
                                            timeSlot.endTime = parseTimeHour(endTimeStr);
                                            timeSlot.endMin = parseTimeMinute(endTimeStr);
                                            timeSlot.isAvailable = (status == "IsAvailable");
                                        }
                                        field.timeSlots.push_back(timeSlot);
                                    }
                                }
                            }
                        }
                    }
                    fields.push_back(field);
                    cout << " Loaded field: " << field.name << " (ID: " << field.id << ")" << endl;
                }
            }
        }
    }
    file.close();
};
