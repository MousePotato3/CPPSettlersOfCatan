#include "Point.h"
#include <iostream>
using namespace std;
Point::Point(int a, int b)
{
     x=a;
     y=b;
}
//This default constructor is used all the time
Point::Point(){}
int Point::getX(){return x;}
int Point::getY(){return y;}
void Point::setX(int a){x=a;}
void Point::setY(int b){y=b;}
bool Point::equals(Point p)
{
     if(p.getX()==x && p.getY()==y)
         return true;
     return false;
}
