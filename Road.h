#ifndef ROAD_H
#define ROAD_H
#include "Point.h"

class Road{
      Point location1;
      Point location2;
      int playerNum;
  public:
      Road();
      Road(Point p1,Point p2,int n);
      Point getLocation1();
      Point getLocation2();
      int getPlayerNum();
};
#endif
