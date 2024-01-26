/*******************************************************************************************
* Structure to hold the type, location, number, and value of each hexagon on the Catan board
*******************************************************************************************/
#include "Hexagon.h"

Hexagon::Hexagon(string t){
	type = t;
	number = -1;
	value = -1;
}
Point Hexagon::getLocation() const { return location; }
string Hexagon::getType() const { return type; }
int Hexagon::getNumber() const { return number; }
int Hexagon::getValue() const { return value; }
void Hexagon::setLocation(const Point& p) { location = p; }
void Hexagon::setNumber(const int n) {
	number = n;
	if (number < 7)
		value = number - 1;
	else
		value = 13 - number;
}
bool Hexagon::operator==(const Hexagon& h) const {
	if (location == h.getLocation()) return true;
	else return false;
}
