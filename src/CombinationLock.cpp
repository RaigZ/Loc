#include "CombinationLock.h"

CombinationLock::CombinationLock(PinName ent, PinName res, PinName p1, PinName p2, array<volatile int, SIZE>) 
: pot1(p1), pot2(p2), _enter(ent), _reset(res) {
    for(unsigned int i = 0; i < SIZE; i++) {
        this->userCombination[i] = DEFAULT_COMB[i];
    }
    _enter.rise(callback(this, &CombinationLock::CombinationCheck));
    _reset.rise(callback(this, &CombinationLock::Reset_ISR));
}

CombinationLock::~CombinationLock() {}

// Enters modified digits into the array 
void CombinationLock::CombinationCheck() {
    if (this->isExistingCombination == false) {
        this->isExistingCombination = true; 
        this->userCombination[0] = pot1.read() + 10;
        this->userCombination[1] = pot2.read() + 10;
    }
    /* the combination does exist, look at the current values of where the potentiometers are stationed
        pass the read potentiometer values 
    */
    else {
        matchCombination();
    }
}

// get input by using array and numbers and validate the input with lock combination
// when run program

// Resets the userCombination back to default
void CombinationLock::Reset_ISR() {
    for(unsigned int i = 0; i < SIZE; i++) {
        this->userCombination[i] = DEFAULT_COMB[i];
    }
}

// Prints all digits within userCombination
void CombinationLock::printCombination() {
    printf("\nCurrent user combination: ");
    for (volatile int* index = begin(this->userCombination); index < end(this->userCombination); ++index) {
        printf("%d ", userCombination[*index]);
        printf("\npot1: %f\npot2: %f\n\n", pot1.read(), pot2.read());
    }
    printf("\n");
}

bool CombinationLock::matchCombination() {
    if(isExistingCombination == true) {
        printf("Digit: %d", (int)pot1.read());
        if (((int)this->pot1.read()) == this->userCombination[0]) {
            //printf("\nDigit satisfied: %d\n", (int)this->pot1.read());
        } else {
            //printf("\nDigit unsatisfied: %d\n", (int)this->pot1.read());
            return false;
        }
        if ((this->pot2.read()) == this->userCombination[1]) {
            //printf("\nDigit satisfied: %d\n", (int)this->pot2.read());
        } else {
            //printf("\nDigit unsatisfied: %d\n", (int)this->pot2.read());
            return false;
        }
    }
    //printf("\nSuccessful combination match!");
    return true;
}

/* 
    Prompt user to enter passcode
*/