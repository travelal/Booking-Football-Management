#ifndef FIELD_ENTITY_H
#define FIELD_ENTITY_H
#include <string>
#include <ctime>
#include <vector>
using namespace std;

class TimeSlot {
    public:
        int id;
        time_t startTime;
        time_t endTime;
        int startMin;
        int endMin;
        bool isAvailable; // true: rented

    TimeSlot(int slotId = 0, time_t start = 0, time_t end = 0, int startMin=0, int endMin=0, bool isAvail = true);
};

class Field {
    public:
        string id;
        string name;
        string location;
        float size;
        float hourlyRate;
        vector<TimeSlot> timeSlots;
        bool isAvailable; // true: all time slot are rented
     
    Field(string id="", string name="", string loc="", double s=0, double rate=0, vector<TimeSlot> slots={}, bool avail=true);
};

#endif