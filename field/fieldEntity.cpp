#include "fieldEntity.h"

TimeSlot::TimeSlot(int slotId, time_t start, time_t end, int startMin, int endMin){
    this->id = slotId;
    this->startTime = start;
    this->endTime = end;
    this->startMin = startMin;
    this->endMin = endMin;
}

Field::Field(string id, string name, string loc, double s, double rate, vector<TimeSlot> slots) {
    this->id = id;
    this->name = name;
    this->location = loc;
    this->size = s;
    this->hourlyRate = rate;
    this->timeSlots = slots;
}