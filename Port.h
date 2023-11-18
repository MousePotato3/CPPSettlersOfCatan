#include "DoublePoint.h"
#include "Point.h"
#include <string>
using namespace std;
#ifndef PORT_H
#define PORT_H

class Port{
      string type;
      Point location;
      DoublePoint portLocations;
  public:
      Port(string t);
      Port();
      string getType();
      Point getLocation();
      DoublePoint getPortLocations();
      void setLocation(Point p);
      void setPortLocations(DoublePoint dp);
};
#endif
