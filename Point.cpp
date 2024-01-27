/*******************************************************************************************
* Structure to hold the coordinates of each intersection on the Catan board
*******************************************************************************************/
#include "Point.h"

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}
// This default constructor is used all the time
Point::Point() {
    x = -1;
    y = -1;
}
int Point::getX() const { return x; }
int Point::getY() const { return y; }
void Point::setX(int a) { x = a; }
void Point::setY(int b) { y = b; }
bool Point::operator==(const Point& p) const
{
    if (x == p.x && y == p.y) return true;
    else return false;
}
bool Point::operator!=(const Point& p) const
{
    if (x == p.x && y == p.y) return false;
    else return true;
}
size_t Point::operator()(const Point& p) const {
    size_t xHash = hash<int>()(p.getX());
    size_t yHash = hash<int>()(p.getY()) << 1;
    return xHash ^ yHash;
}
