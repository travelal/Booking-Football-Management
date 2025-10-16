#include "fieldEntity.h"

TimeSlot::TimeSlot(int slotId, time_t start, time_t end, int startMin, int endMin, bool isAvail){
    this->id = slotId;
    this->startTime = start;
    this->endTime = end;
    this->startMin = startMin;
    this->endMin = endMin;
    this->isAvailable = isAvail;
}

Field::Field(string id, string name, string loc, double s, double rate, vector<TimeSlot> slots, bool avail) {
    this->id = id;
    this->name = name;
    this->location = loc;
    this->size = s;
    this->hourlyRate = rate;
    this->timeSlots = slots;
    this->isAvailable = avail;
}