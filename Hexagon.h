#ifndef HEXAGON_H
#define HEXAGON_H
#include <string>
#include "Point.h"
using namespace std;

class Hexagon
{
      Point location;
      string type;
      int number;
      int value;
  public:
      Hexagon();
      Hexagon(string t);
      Point getLocation();      
      string getType();
      int getNumber();
      int getValue();
      void setLocation(Point p);
      void setNumber(int n);
      bool equals(Hexagon h);
};
#endif
