#ifndef DOUBLEPOINT_H
#define DOUBLEPOINT_H
#include "Point.h"

class DoublePoint{
      Point p1;
      Point p2;
  public:
      DoublePoint(Point a, Point b);
      DoublePoint();
      Point getP1();
      Point getP2();
      bool equals(DoublePoint p);
};
#endif
