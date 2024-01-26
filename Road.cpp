/*******************************************************************************************
* Structure to hold the location and player number of each road on the Catan board
*******************************************************************************************/
#include "Road.h"

Road::Road(Point p1, Point p2, int n)
{
    location1 = p1;
    location2 = p2;
    playerNum = n;
}
Point Road::getLocation1() const { return location1; }
Point Road::getLocation2() const { return location2; }
int Road::getPlayerNum() const { return playerNum; }
