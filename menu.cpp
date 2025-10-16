#include <iostream>
#include "field/fieldFunction.h"
#include "booking/bookingFunction.h"
using namespace std;

void fieldMenu();
void bookingMenu();
void runMenu(){
    int choose;
    do {
        cout<<"===== FOOTBALL FIELD BOOKING SYSTEM =====\n";
        cout<<"1.Field Management\n";
        cout<<"2.Booking Management\n";
        cout<<"0.Exit\n";
        cout<<"Choose option: ";
        cin>>choose;
        
        switch (choose){
            case 1: fieldMenu();
            break;
            case 2: bookingMenu();
            break;
            case 0: cout<<"exiting project";
            break;
            default: cout<<"Wrong option";
        };
    }while(choose!=0);
};