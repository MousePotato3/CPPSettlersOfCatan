#ifndef SETTLEMENT_H
#define SETTLEMENT_H
#include "Point.h"

class Settlement{
      Point location;
      int playerNum;
  public:
      Settlement();
      Settlement(Point p,int n);
      Point getLocation();
      int getPlayerNum();
};
#endif
