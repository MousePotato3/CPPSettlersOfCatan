#ifndef HEXAGON_H
#define HEXAGON_H
#include <iostream>
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
      Hexagon(string t);
      Point getLocation() const;      
      string getType() const;
      int getNumber() const;
      int getValue() const;
      void setLocation(const Point& p);
      void setNumber(const int n);
      bool operator==(const Hexagon& h) const;
};
#endif
