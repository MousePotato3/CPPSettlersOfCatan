#include "DoublePoint.h"
DoublePoint::DoublePoint(Point a, Point b)
{
     p1=a;
	 p2=b;
}
DoublePoint::DoublePoint(){}
Point DoublePoint::getP1(){return p1;}
Point DoublePoint::getP2(){return p2;}
bool DoublePoint::equals(DoublePoint k)
{
     if((p1.equals(k.getP1()) && p2.equals(k.getP2())) || (p1.equals(k.getP2()) && p2.equals(k.getP1())))
          return true;
     return false;
}
