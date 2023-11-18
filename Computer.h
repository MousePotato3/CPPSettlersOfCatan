#ifndef COMPUTER_H
#define COMPUTER_H
#include "Player.h"
#include <vector>
class Computer: public Player {
      vector<Point> hexIntersections;
  public:
      Computer(Board startBoard, int n);
      Computer();
      Point placeFirstSettlement();
      Point placeFirstRoad(Point p);
      Point placeSecondSettlement();
      Point placeSecondRoad(Point p);
      void discard(int turn);
      vector<int> canPlaceCity();
      vector<int> canPlaceSettlement();
      vector<int> canPlaceRoad();
      int placeCity(int turn);
      int placeSettlement(vector<Point> possibleSettlements, int turn);
      int placeRoad(vector<DoublePoint> possibleRoads, int turn);
      int takeTurn(vector<DoublePoint> roadPoints, vector<Point> settlePoints, int turn);
};
#endif
