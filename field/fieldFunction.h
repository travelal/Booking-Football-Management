#ifndef FIELD_FUNCTION_H
#define FIELD_FUNCTION_H
#include <string>
#include <vector>
#include "fieldEntity.h"
using namespace std;

extern vector<Field> fields;

void fieldMenu();
void addField();
void editField();
void editBasicField(string id);
void editFieldStatus(string id);
void editTimeSlots();
void editTimeSlotStatus();
void deleteField();
void deleteFieldBasic();
void deleteTimeSlots();
void displayField();
void displayAvailableField();
void findField();
void saveFileOfField();
void loadFileOfField();
#endif