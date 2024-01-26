/*******************************************************************************************
* Structure to hold the numbers rolled on each 6-sided die, and their sum
*******************************************************************************************/
#include "Dice.h"

Dice::Dice() {
    srand ( (unsigned int)time(NULL) );
    d1 = 0;
    d2 = 0;
    sum = 0;
}
void Dice::resetDice() { srand((unsigned int)time(NULL)); }
int Dice::rollDice() {
    d1 = rand() % 6 + 1;
    d2 = rand() % 6 + 1;
    sum = d1 + d2;
    return sum;
}
int Dice::getRoll() const {
    return sum;
}
