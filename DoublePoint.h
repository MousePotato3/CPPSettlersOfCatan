#ifndef DOUBLEPOINT_H
#define DOUBLEPOINT_H
#include "Point.h"
using namespace std;

class DoublePoint{
      Point p1;
      Point p2;
  public:
      DoublePoint(Point a, Point b);
      DoublePoint();
      Point getP1() const;
      Point getP2() const;
      bool operator==(const DoublePoint& d) const;
      bool operator!=(const DoublePoint& d) const;
//    size_t operator()(const DoublePoint& d) const;
};
#endif
