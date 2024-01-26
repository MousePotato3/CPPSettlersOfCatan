#ifndef PORT_H
#define PORT_H
#include <iostream>
#include <string>
#include "DoublePoint.h"
#include "Point.h"
using namespace std;

class Port{
      string type;
      Point location;
      DoublePoint portLocations;
  public:
      Port(string t);
      string getType() const;
      Point getLocation() const;
      DoublePoint getPortLocations() const;
      void setLocation(const Point& p);
      void setPortLocations(const DoublePoint& dp);
};
#endif
