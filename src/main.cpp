/*----------------------------------------------------------------------------
    CS435 Project - Locker
 ----------------------------------------
Smart Lock
*----------------------------------------------------------------------------*/

#include "InterruptIn.h"
#include "ThisThread.h"
#include "mbed.h" 
#include "NHD_0216HZ.h"
#include <string>

#define SIZE 2

int default_combination[SIZE] = { 0,0 };
int user_combination[SIZE] = {};
int volatile bitmode = 0;

AnalogIn meter1(A0);
AnalogIn meter2(A1);
InterruptIn enter(D2);
InterruptIn reset(D3);

int checkCombinationStatus(int comb[], int ent[], int bit) {
    switch(bit) {
        // enter
        case 1:
            for(int i = 0; i < SIZE; i++) {
                if(comb[i] != ent[i]) {
                    print_lcd("Incorrect");
                    ThisThread::sleep_for(400);
                    return 0;
                }
            }
            print_lcd("Correct");
            ThisThread::sleep_for(1000);
            return 0;
            break;
        // reset
        case 2:
            for(int i = 0; i < SIZE; i++) {
                comb[i] = ent[i]; 
            }
            bit = 0; 
            print_lcd("Set");
            ThisThread::sleep_for(1000);
            return 0;
    }
    return 0;
}

void enter_ISR() {
    bitmode = 1;
}

void reset_ISR() {
    bitmode = 2;
}

int mapVal(float m1) {
    if(m1 >= 0.000 && m1 <= 0.09) {
        return 0;
    }
    else if(m1 > 0.09 && m1 <= 0.19) {
        return 1;    
    }
    else if(m1 > 0.19 && m1 <= 0.29) {
        return 2;    
    }
    else if(m1 > 0.29 && m1 <= 0.39) {
        return 3;    
    }
    else if(m1 > 0.39 && m1 <= 0.49) {
        return 4;    
    }
    else if(m1 > 0.49 && m1 <= 0.59) {
        return 5;    
    }
    else if(m1 > 0.59 && m1 <= 0.69) {
        return 6;    
    }
    else if(m1 > 0.69 && m1 <= 0.79) {
        return 7;    
    }
    else if(m1 > 0.79 && m1 <= 0.89) {
        return 8;    
    }
    else if(m1 > 0.89 && m1 <= 10.0) {
        return 9;    
    }    
    return -1;
} 

int main() {     
    init_spi();
    init_lcd();
    set_cursor(0, 1);
    enter.rise(enter_ISR);
    reset.rise(reset_ISR);
    while(true) {
        printf("m1: %d, m2: %d\n", mapVal(meter1.read()), mapVal(meter2.read()));
        int potMap = mapVal(meter1.read());
        int potMap2 = mapVal(meter2.read());   
        user_combination[0] = potMap;
        user_combination[1] = potMap2;

        char const* pchar = to_string(potMap).c_str();
        char const* pchar2 = to_string(potMap2).c_str();
    
        bitmode = checkCombinationStatus(default_combination, user_combination, bitmode);
        ThisThread::sleep_for(300); 
        write_cmd(0x01);
    }
}