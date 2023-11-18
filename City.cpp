#include "City.h"
#include <iostream>
using namespace std;
City::City(){cerr<<"Inside empty city default constructor"<<endl;}
City::City(Point p,int n)
{
      location=p;
      playerNum=n;
}
Point City::getLocation(){return location;}
int City::getPlayerNum(){return playerNum;}
