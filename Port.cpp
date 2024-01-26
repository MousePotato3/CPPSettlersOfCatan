/*******************************************************************************************
* Structure to hold the type and location of each port on the Catan board
*******************************************************************************************/
#include "Port.h"

Port::Port(string t) { type = t; }
string Port::getType() const { return type; }
Point Port::getLocation() const { return location; }
DoublePoint Port::getPortLocations() const { return portLocations; }
void Port::setLocation(const Point& p) { location = p; }
void Port::setPortLocations(const DoublePoint& dp) { portLocations = dp; }
