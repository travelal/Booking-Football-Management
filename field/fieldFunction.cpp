#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "fieldEntity.h"
#include "fieldFunction.h"
#include "../booking-field/bookingField.h"
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
        f.timeSlots.push_back(TimeSlot(i + 1, startTime, endTime, startMin, endMin));
    }

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
                    cout << "2. Edit time slots\n";
                    cout << "0. Back\n";
                    cout << "Choice One: ";
                    cin >> choice;
                    switch ( choice ) {
                        case 1:
                            editBasicField(id);
                            break;
                        case 2:
                            editTimeSlots();
                            break;
                    }
                } while (choice !=0);
        }
    }
    saveFileOfField();
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
    }
    cout << "Field with ID" << id << "not found" << endl;        
    saveFileOfField();
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
                         << timeSlot.endTime << "h" << timeSlot.endMin <<"p" << endl;
                    
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

void displayField(){
    if (fields.empty()){
        cout << "No Field" << endl;
        return;
    }
    
    for( auto &f : fields){
        cout <<f.id<< " - " << f.name << "-" << f.location
             << " -  " << f.size << " - " <<f.hourlyRate <<"$/h" << endl;

        
        cout << " Time Slots: " << endl;
        if (f.timeSlots.empty()){
            cout << "No time available" << endl;
        } else {
            int i;
            for (i=0; i < f.timeSlots.size(); i++){
                cout << f.timeSlots[i].id << "." << f.timeSlots[i].startTime <<"h"
                    <<f.timeSlots[i].startMin <<"p"<< " - " << f.timeSlots[i].endTime 
                    << "h" <<f.timeSlots[i].endMin<< "p" << endl;
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
            << f.hourlyRate << "$/h" << endl;
        }
    }
};


void saveFieldToFile(const vector<Field>& fields) {
    ofstream file("data/field.txt");
    if (!file) {
        cout << "Failed to open field.txt for writing.\n";
        return;
    }
    
    for (auto &f : fields) {
        file << "Field." << f.id << ". "
             << f.name << " - " << f.location << " - "
             << f.size << " - " << f.hourlyRate << "$/h - "
             << "\n";
        
        file << f.timeSlots.size() << " slot:\n";
        
        for (const TimeSlot &timeSlot : f.timeSlots) {
            auto formatMinute = [](int m) {
                string s = to_string(m);
                if (m < 10) s = "0" + s;
                return s;
            };
            
            file << timeSlot.id << ". "
                 << timeSlot.startTime << "h" << formatMinute(timeSlot.startMin) << "p"
                 << " - "
                 << timeSlot.endTime << "h" << formatMinute(timeSlot.endMin) << "p"
                 << "\n";
            }
            file << "\n";
        }
        file.close();
        cout << "Field data saved successfully!" << endl;
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
             << f.size << " - " << f.hourlyRate << "$/h\n";

        file << f.timeSlots.size() << " slot:\n";

        for (const TimeSlot &timeSlot : f.timeSlots) {
            auto formatMinute = [](int m) {
                return (m < 10) ? "0" + to_string(m) : to_string(m);
            };

            file << timeSlot.id << ". "
                 << timeSlot.startTime << "h" << formatMinute(timeSlot.startMin) << "p"
                 << " - "
                 << timeSlot.endTime << "h" << formatMinute(timeSlot.endMin) << "p\n";
        }

        file << "\n"; // Thêm dòng trống giữa các field
    }

    file.close();
    cout << "Field data saved successfully!\n";
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

                if (dash1 != string::npos && dash2 != string::npos &&
                    dash3 != string::npos) {

                    field.name = fieldInfo.substr(0, dash1);
                    field.location = fieldInfo.substr(dash1 + 3, dash2 - dash1 - 3);
                    string sizeStr = fieldInfo.substr(dash2 + 3, dash3 - dash2 - 3);
                    string rateStr = fieldInfo.substr(dash3 + 3);

                    field.size = (sizeStr.empty()) ? 0.0f : stof(sizeStr);
                    field.hourlyRate = (rateStr.empty()) ? 0.0f : stof(rateStr);
                    fields.push_back(field);
                }
            }
        }
    }
    file.close();
};

Field loadFieldFromFileById(string fieldId){
    ifstream file("data/field.txt");
    Field f;
    if (!file.is_open()) 
    return f;
    
    string line;
    while (getline(file, line)){
        if (line.empty())
        continue;
        Field temp = parseFieldLine(file, line);
        if (temp.id == fieldId){
            f = temp;
            break;
        }
    }
    file.close();
    return f;
}

Field parseFieldHeader(const string &line) {
    Field field;
    if (line.find("Field.") != 0) return field;

    size_t pos1 = line.find('.');
    size_t pos2 = line.find('.', pos1 + 1);
    if (pos1 == string::npos || pos2 == string::npos) return field;

    field.id = line.substr(pos1 + 1, pos2 - pos1 - 1);
    string fieldInfo = line.substr(pos2 + 1);

    size_t dash1 = fieldInfo.find(" - ");
    size_t dash2 = fieldInfo.find(" - ", dash1 + 3);
    size_t dash3 = fieldInfo.find(" - ", dash2 + 3);

    if (dash1 == string::npos || dash2 == string::npos ||
        dash3 == string::npos) return field;

    field.name = fieldInfo.substr(0, dash1);
    field.location = fieldInfo.substr(dash1 + 3, dash2 - dash1 - 3);
    field.size = stof(fieldInfo.substr(dash2 + 3, dash3 - dash2 - 3));

    string rateStr = fieldInfo.substr(dash3 + 3);
    size_t dollarPos = rateStr.find('$');
    if (dollarPos != string::npos) {
        rateStr = rateStr.substr(0, dollarPos);
    }
    field.hourlyRate = stof(rateStr);
    
    return field;
}

void parseTimeSlots(ifstream &file, Field &field) {
    string slotLine;
    if (!getline(file, slotLine)) return;

    size_t slotPos = slotLine.find(" slot:");
    if (slotPos == string::npos) return;

    int slotCount = stoi(slotLine.substr(0, slotPos));

    for (int i = 0; i < slotCount; i++) {
        if (!getline(file, slotLine) || slotLine.empty()) continue;

        size_t dotPos = slotLine.find('.');
        size_t dashPos = slotLine.find(" - ");
        if (dotPos == string::npos || dashPos == string::npos) continue;

        TimeSlot slot;
        slot.id = stoi(slotLine.substr(0, dotPos));

        // Parse start time
        string timeInfo = slotLine.substr(dotPos + 1, dashPos - dotPos - 1);
        size_t hPos = timeInfo.find('h');
        size_t pPos = timeInfo.find('p', hPos);
        slot.startTime = stoi(timeInfo.substr(0, hPos));
        slot.startMin = stoi(timeInfo.substr(hPos + 1, pPos - hPos - 1));

        // Parse end time
        timeInfo = slotLine.substr(dashPos + 3);
        hPos = timeInfo.find('h');
        pPos = timeInfo.find('p', hPos);
        slot.endTime = stoi(timeInfo.substr(0, hPos));
        slot.endMin = stoi(timeInfo.substr(hPos + 1, pPos - hPos - 1));

        field.timeSlots.push_back(slot);
    }
}

Field parseFieldLine(ifstream &file, const string &line) {
    Field field = parseFieldHeader(line);
    if (!field.id.empty()) {
        parseTimeSlots(file, field);
    }
    return field;
}

time_t parseTime(const string &timeStr) {
    tm t = {};
    istringstream ss(timeStr);
    ss >> get_time(&t, "%H:%M");
    if (ss.fail()) return 0;
    return mktime(&t);
};

vector<Field> loadFieldFromFileInVectorType() {
    vector<Field> fields;
    ifstream file("data/field.txt");
    if (!file) {
        cout << "Cannot open file " << endl;
        return fields;
    }

    string line;
    Field currentField;
    bool readingSlots = false;
    int expectedSlots = 0;
    int slotsRead = 0;
    
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Nếu bắt đầu một field mới
        if (line.find("Field.") == 0) {
            // Lưu field trước đó nếu có
            if (!currentField.id.empty()) {
                fields.push_back(currentField);
            }
            
            // Parse field mới
            currentField = Field();
            currentField.timeSlots.clear();
            
            // Field.1. FieldA - Location - 11 - 20$/h - IsAvailable
            size_t firstDot = line.find('.');
            size_t secondDot = line.find('.', firstDot + 1);
            
            if (firstDot != string::npos && secondDot != string::npos) {
                currentField.id = line.substr(firstDot + 1, secondDot - firstDot - 1);
                
                string rest = line.substr(secondDot + 2); // +2 để bỏ khoảng trắng
                
                // Parse name, location, size, rate
                size_t dash1 = rest.find(" - ");
                size_t dash2 = rest.find(" - ", dash1 + 3);
                size_t dash3 = rest.find(" - ", dash2 + 3);
                
                if (dash1 != string::npos && dash2 != string::npos && 
                    dash3 != string::npos) {
                    
                    currentField.name = rest.substr(0, dash1);
                    currentField.location = rest.substr(dash1 + 3, dash2 - dash1 - 3);
                    currentField.size = stof(rest.substr(dash2 + 3, dash3 - dash2 - 3));
                    
                    string rateStr = rest.substr(dash3 + 3);
                    size_t dollarPos = rateStr.find('$');
                    if (dollarPos != string::npos) {
                        rateStr = rateStr.substr(0, dollarPos);
                    }
                    currentField.hourlyRate = stof(rateStr);
                }
            }
            readingSlots = false;
            expectedSlots = 0;
            slotsRead = 0;
        }
        // Nếu là dòng số lượng slot
        else if (line.find(" slot:") != string::npos) {
            string slotCountStr = line.substr(0, line.find(" slot:"));
            expectedSlots = stoi(slotCountStr);
            readingSlots = true;
            slotsRead = 0;
        }
        // Nếu đang đọc slots
        else if (readingSlots && slotsRead < expectedSlots) {
            // Parse slot: "1. 8h00p - 9h00p "
            stringstream ss(line);
            int slotId;
            char dot, h1, p1, dash, h2, p2;
            int startH, startM, endH, endM;
            string status;
            
            ss >> slotId >> dot;
            ss >> startH >> h1 >> startM >> p1;
            ss.ignore(3); // bỏ " - "
            ss >> endH >> h2 >> endM >> p2;
            
            
            currentField.timeSlots.push_back(TimeSlot(slotId, startH, endH, startM, endM));
            slotsRead++;
        }
    }
    
    // Thêm field cuối cùng
    if (!currentField.id.empty()) {
        fields.push_back(currentField);
    }
    
    file.close();
    return fields;
}