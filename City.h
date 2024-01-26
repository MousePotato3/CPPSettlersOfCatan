#ifndef CITY_H
#define CITY_H
#include <iostream>
#include "Point.h"
using namespace std;

class City{
      Point location;
      int playerNum;
  public:
      City(Point p,int n);
      Point getLocation() const;
      int getPlayerNum() const;
};
#endif
