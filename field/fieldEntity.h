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
    TimeSlot(int slotId = 0, time_t start = 0, time_t end = 0, int startMin=0, int endMin=0);
};

class Field {
    public:
        string id;
        string name;
        string location;
        float size;
        float hourlyRate;
        vector<TimeSlot> timeSlots;     
    Field(string id="", string name="", string loc="", double s=0, double rate=0, vector<TimeSlot> slots={});
};

#endif