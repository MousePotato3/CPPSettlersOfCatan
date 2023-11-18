#ifndef CITY_H
#define CITY_H
#include "Point.h"

class City{
      Point location;
      int playerNum;
  public:
      City();
      City(Point p,int n);
      Point getLocation();
      int getPlayerNum();
};
#endif
