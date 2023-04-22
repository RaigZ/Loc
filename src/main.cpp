/*----------------------------------------------------------------------------
    CS435 Project - Locker
 ----------------------------------------
Smart Lock
*----------------------------------------------------------------------------*/

#include "ThisThread.h"
#include "mbed.h"
#include <array>

#include "CombinationLock.h"

int main() {
    array<volatile int, 2> userComb;
    CombinationLock cl(A0, A1, A2, A3, userComb);
    while(true) {
        cl.printCombination();
        ThisThread::sleep_for(500ms);
    }
}