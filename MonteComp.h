#ifndef MONTECOMP_H
#define MONTECOMP_H
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "Player.h"
#include "Simulation.h"
using namespace std;

class MonteComp: public Player {
      vector<Point> hexIntersections;
      string actionType;
  public:
      MonteComp(int n, string c, string t, int p);
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
	  int runSimulation (Board testBoard, string actionType, int playerResources[], int playerNumPoints) const;
};
#endif
