/*******************************************************************************************
* Structure to hold the location and player number of each settlement on the Catan board
*******************************************************************************************/
#include "Settlement.h"

Settlement::Settlement(Point p,int n)
{
    location = p;
    playerNum = n;
}
Point Settlement::getLocation() const { return location; }
int Settlement::getPlayerNum() const { return playerNum; }
