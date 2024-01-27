/*******************************************************************************************
* Structure to hold 2 Point variables at once
*******************************************************************************************/
#include "DoublePoint.h"

DoublePoint::DoublePoint(Point p1, Point p2)
{
    this->p1 = p1;
    this->p2 = p2;
}
DoublePoint::DoublePoint(){}
Point DoublePoint::getP1() const { return p1; }
Point DoublePoint::getP2() const { return p2; }
bool DoublePoint::operator==(const DoublePoint& d) const
{
    if ((p1 == d.p1 && p2 == d.p2) || (p1 == d.p2 && p2 == d.p1)) return true;
    else return false;
}
bool DoublePoint::operator!=(const DoublePoint& d) const
{
    if ((p1 == d.p1 && p2 == d.p2) || (p1 == d.p2 && p2 == d.p1)) return false;
    else return true;
}
/*size_t DoublePoint::operator()(const DoublePoint& d) const {
    size_t p1Hash = hash<Point>()(d.getP1());
    size_t p2Hash = hash<Point>()(d.getP2()) << 1;
    return xHash ^ yHash;
}
*/
