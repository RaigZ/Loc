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
#include "Servo.h"

#define SIZE 4

int default_combination[SIZE] = { 0,0,0,0 };
int user_combination[SIZE] = {};
int volatile bitmode = 0;
bool enterBit = 0; 
AnalogIn meter1(A4); //PA_0
AnalogIn meter2(A2); //PA_1
AnalogIn meter3(A0); //PC_0
AnalogIn meter4(A1); //PC_1 
InterruptIn enter(D2); //PA_10
InterruptIn reset(D3); //PC_3
Servo servo(D4);   
DigitalOut red(D5);
DigitalOut green(D8);    

int checkCombinationStatus(int comb[], int ent[], int bit) {
    switch(bit) {
        // enter
        case 1:
            for(int i = 0; i < SIZE; i++) {
                if(comb[i] != ent[i]) {
                    set_cursor(0, 1);
                    print_lcd("Incorrect: ");
                    enterBit = 0;
                    red = 1;
                    printf("red: %d\n", (int)red);
                    for(int j = 5; j > 0; j--) {
                        printf("%d\n", j);
                        print_lcd(to_string(j).c_str());
                        ThisThread::sleep_for(1000);
                    }
                    ThisThread::sleep_for(400);
                    red = 0;
                    printf("red: %d\n", (int)red);
                    return 0;
                }
            }
            enterBit = 1;
            green = 1;
            printf("green: %d\n", (int)green);
            set_cursor(0, 1);
            print_lcd("Correct");
            servo.write(90);
            ThisThread::sleep_for(1000);
            servo.write(-90);
            green = 0;
            printf("green: %d\n", (int)green);
            break;
        // reset
        case 2:
            if(enterBit) {
                for(int i = 0; i < SIZE; i++) {
                    comb[i] = ent[i]; 
                }
                enterBit = 0; 
                set_cursor(0, 1);
                print_lcd("Set");            
                ThisThread::sleep_for(1000);
                break;
            }
            else {
                set_cursor(0, 1);
                print_lcd("Cannot Set");
                printf("Cannot Set\n"); 
                ThisThread::sleep_for(1000);
            }
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
    enter.rise(enter_ISR);
    reset.rise(reset_ISR);
    servo.attach(SERVO_POS_CENTER);
    
    while(true) {
        printf("%d %d %d %d\n", mapVal(meter1.read()), mapVal(meter2.read()), mapVal(meter3.read()), mapVal(meter4.read()));
        int potMap = mapVal(meter1.read());
        int potMap2 = mapVal(meter2.read());   
        int potMap3 = mapVal(meter3.read());   
        int potMap4 = mapVal(meter4.read());   

        user_combination[0] = potMap;
        user_combination[1] = potMap2;
        user_combination[2] = potMap3;
        user_combination[3] = potMap4;

        printf("SERVO: %d\n", servo.read());
        char const* pchar = to_string(potMap).c_str();
        char const* pchar2 = to_string(potMap2).c_str();
        char const* pchar3 = to_string(potMap3).c_str();
        char const* pchar4 = to_string(potMap4).c_str();
        std::string s = (to_string(potMap) + to_string(potMap2) + to_string(potMap3) + to_string(potMap4));
        print_lcd(s.c_str());
        bitmode = checkCombinationStatus(default_combination, user_combination, bitmode);
        ThisThread::sleep_for(300); 
        write_cmd(0x01);
    }
}