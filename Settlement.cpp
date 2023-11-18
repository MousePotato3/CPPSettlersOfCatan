#include "Settlement.h"
#include <iostream>
using namespace std;
Settlement::Settlement(Point p,int n)
{
      location=p;
      playerNum=n;
}
Settlement::Settlement(){cerr<<"Inside empty RandComp default constructor"<<endl;}
Point Settlement::getLocation(){return location;}
int Settlement::getPlayerNum(){return playerNum;}
