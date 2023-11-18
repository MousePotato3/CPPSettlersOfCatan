#include "Road.h"
#include <iostream>
using namespace std;
Road::Road(){cerr<<"Inside empty road default constructor"<<endl;}
Road::Road(Point p1,Point p2,int n)
{
      location1=p1;
      location2=p2;
      playerNum=n;
}
Point Road::getLocation1(){return location1;}
Point Road::getLocation2(){return location2;}
int Road::getPlayerNum(){return playerNum;}

