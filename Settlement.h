#ifndef SETTLEMENT_H
#define SETTLEMENT_H
#include <iostream>
#include "Point.h"
using namespace std;

class Settlement{
      Point location;
      int playerNum;
  public:
      Settlement(Point p,int n);
      Point getLocation() const;
      int getPlayerNum() const;
};
#endif
