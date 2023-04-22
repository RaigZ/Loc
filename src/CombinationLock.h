#include "AnalogIn.h"
#include "PinNames.h"
#include "ext/DebounceIn.h"
#include "mbed.h"
#include <array>
/*
    A combination lock may consist of:
    - Default values set within a lock's combination
    - Object representing the combination bounded upon the end-user
    - Adjust numbers using potentiometers
*/
const int SIZE = 2; 
const array<volatile int, SIZE> DEFAULT_COMB = {0, 0};

class CombinationLock {
private:
  array<volatile int, SIZE> userCombination; // Currently set to size of 2
  AnalogIn pot1, pot2;
  DebounceIn _enter, _reset;
  bool isExistingCombination = false;

public:
  CombinationLock(PinName, PinName, PinName, PinName, array<volatile int, SIZE>);
  ~CombinationLock();

  void CombinationCheck(); // enters the full combination (manual input for users using
                    // potentiometers)
  void Reset_ISR(); // resets the combination (turns combination back to
                    // DEFAULT_COMB)
  void printCombination();

  bool matchCombination();
};

/*
    Considerations:

    Can also allow user to randomize combinations on lock.
    Just use rand...
*/