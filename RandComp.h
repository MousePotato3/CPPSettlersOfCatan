#ifndef RANDCOMP_H
#define RANDCOMP_H
#include "Player.h"
#include <vector>
class RandComp: public Player {
      vector<Point> hexIntersections;
  public:
	  RandComp(Board startBoard, int n, bool b);
	  RandComp();
      Point placeFirstSettlement();
      Point placeFirstRoad(Point p);
      Point placeSecondSettlement();
      Point placeSecondRoad(Point p);
      void discard(int turn);
      vector<int> canPlaceCity();
      vector<int> canPlaceSettlement();
      vector<int> canPlaceRoad();
      int placeCity(int playerResources[][5], int playerNumPoints[], int turn);
      int placeSettlement(vector<Point> possibleSettlements, int playerResources[][5], int playerNumPoints[], int turn);
      int placeRoad(vector<DoublePoint> possibleRoads, int playerResources[][5], int playerNumPoints[], int turn);
      int takeTurn(vector<DoublePoint> roadPoints, vector<Point> settlePoints, int playerResources[][5], int playerNumPoints[], int turn);
};
#endif
