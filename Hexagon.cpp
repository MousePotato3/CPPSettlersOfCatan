#include "Hexagon.h"
#include <iostream>
Hexagon::Hexagon(){cerr<<"Inside empty hexagon default constructor"<<endl;}
Hexagon::Hexagon(string t){type=t;}
Point Hexagon::getLocation(){return location;}
string Hexagon::getType(){return type;}
int Hexagon::getNumber(){return number;}
int Hexagon::getValue(){return value;}
void Hexagon::setLocation(Point p){location=p;}
void Hexagon::setNumber(int n){
	number=n;
	if(number<7)
		value=number-1;
	else
		value=13-number;
}
bool Hexagon::equals(Hexagon h){
	if(location.equals(h.getLocation()))
		return true;
	return false;
}
