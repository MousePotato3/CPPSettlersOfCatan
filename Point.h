#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <string>
using namespace std;

class Point{
      int x;
      int y;
  public:
      Point(int a, int b);
      Point();
      int getX() const;
      int getY() const;
      void setX(int a);
      void setY(int b);
      bool operator==(const Point& p) const;
      bool operator!=(const Point& p) const;
      size_t operator()(const Point& p) const;
      string toString() ;
};
#endif
