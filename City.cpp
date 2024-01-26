/*******************************************************************************************
* Structure to hold the location and player number of each city on the Catan board
*******************************************************************************************/
#include "City.h"

City::City(Point p,int n)
{
    location = p;
    playerNum = n;
}
Point City::getLocation() const { return location; }
int City::getPlayerNum() const { return playerNum; }
