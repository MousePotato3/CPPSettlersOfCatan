#include <stdlib.h>
#include <time.h>
#include "Dice.h"
Dice::Dice() {srand ( (unsigned int)time(NULL) );}
void Dice::resetDice() {srand ( (unsigned int)time(NULL) );}
int Dice::rollDice(){
    d1=rand()%6+1;
    d2=rand()%6+1;
    sum=d1+d2;
    return sum;
}
int Dice::getRoll(){
    return sum;
}
