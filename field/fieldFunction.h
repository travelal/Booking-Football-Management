#ifndef FIELD_FUNCTION_H
#define FIELD_FUNCTION_H
#include <string>
#include <vector>
#include <fstream>
#include "fieldEntity.h"
using namespace std;

extern vector<Field> fields;

void fieldMenu();
void addField();
void editField();
void editBasicField(string id);
void editTimeSlots();
void deleteField();
void deleteFieldBasic(string fieldId);
void deleteTimeSlots(string fieldId);
void displayField();
void displayAvailableField();
void findField();
void saveFileOfField();
void loadFileOfField();
Field loadFieldFromFileById(string fieldId);
Field parseFieldLine(ifstream &file, const string &line);
vector<Field> loadFieldFromFileInVectorType();
void saveFieldToFile(const vector<Field>& fields);

#endif