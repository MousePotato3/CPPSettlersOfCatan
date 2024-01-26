#ifndef RANDCOMP_H
#define RANDCOMP_H
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "Player.h"
using namespace std;

class RandComp: public Player {
      int tempResources[5] = { 0,0,0,0,0 };
      vector<Point> hexIntersections;
  public:
	  RandComp(int n, string c, string t, int p, bool v);
      int getHexValue(Point p);
      int getRoadValue(DoublePoint d);
      Point chooseInitialSettlementLocation(Board b);
      Point chooseInitialRoadLocation(Point p);
      void discard();
      int getPlayerToRob();
      Point getPointToBlock(int playerToRob);
      vector<int> cityPortResources();
      vector<int> settlementPortResources();
      vector<int> roadPortResources();
      int placeCity(vector<Point> cityPoints);
      int placeSettlement(vector<Point> settlementPoints);
      int placeRoad(vector<DoublePoint> roadPoints);
      Board takeTurn(Board b);
};
#endif
