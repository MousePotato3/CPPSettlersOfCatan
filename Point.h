#ifndef POINT_H
#define POINT_H

class Point{
      int x;
      int y;
  public:
      Point(int a, int b);
      Point();
      int getX();
      int getY();
      void setX(int a);
      void setY(int b);
      bool equals(Point p);
};
#endif
