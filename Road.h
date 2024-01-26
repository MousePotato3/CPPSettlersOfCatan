#ifndef ROAD_H
#define ROAD_H
#include <iostream>
#include "Point.h"
using namespace std;

class Road{
      Point location1;
      Point location2;
      int playerNum;
  public:
      Road(Point p1,Point p2,int n);
      Point getLocation1() const;
      Point getLocation2() const;
      int getPlayerNum() const;
};
#endif
