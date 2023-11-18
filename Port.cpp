#include "Port.h"
#include <iostream>
Port::Port(){cerr<<"Inside empty RandComp default constructor"<<endl;}
Port::Port(string t){type=t;}
string Port::getType(){return type;}
Point Port::getLocation(){return location;}
DoublePoint Port::getPortLocations(){return portLocations;}
void Port::setLocation(Point p){location=p;}
void Port::setPortLocations(DoublePoint dp){portLocations=dp;}
