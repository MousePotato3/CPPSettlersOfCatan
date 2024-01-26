#ifndef DICE_H
#define DICE_H
#include <stdlib.h>
#include <time.h>

class Dice{
      int d1;
      int d2;
      int sum;
  public:
	  Dice();
	  void resetDice();
      int rollDice();
      int getRoll() const;
};
#endif
