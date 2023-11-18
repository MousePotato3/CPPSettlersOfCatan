#ifndef PLAYER_H
#define PLAYER_H
#include "Point.h"
#include "Board.h"
#include <string>
#include <vector>
using namespace std;
class Player{
  protected:
	  Board board;
	  vector<Settlement> settlements;
	  vector<City> cities;
	  vector<Road> roads;
	  vector<Point> newCityPoints;
	  vector<Point> newSettlePoints;
	  vector<DoublePoint> newRoadPoints;
	  int resources[5];
	  int tradeRates[5];
	  int playerNum;
	  int points;
	  bool isVisible;
  public:
	  Player();
	  int getPoints();
	  int getNumSettlements();
	  int getNumCities();
	  int getNumRoads();
	  vector<Settlement> getSettlements();
	  vector<City> getCities();
	  vector<Road> getRoads();
	  void getResources(int resourcesToReplace[]);
	  vector<Point> getCityPoints();
      vector<Point> getSettlePoints();
      vector<DoublePoint> getRoadPoints();
	  void setResources(int newResources[]);
	  void setPoints(int newPoints);
	  void setVisible(bool b);
      void resetBoard(Board b);
	  void resetData();
      void gainResource(int i);
      void loseResource(int i);
      void addSettlement(Point p, int n);
      void addCity(Point p, int n, bool removeSettlement);
      void addRoad(Point p1, Point p2, int n);
	  string getResourceType(int i);
      void addResource(string t);
	  int findSettlementIndex(Point p);
      int getTotalResources();
      int getTotalSettlements();
      void gainPortPower(string t);
      int getResourceToSteal();
	  void printResources(int turn);
      
      virtual Point placeFirstSettlement() =0;
      virtual Point placeFirstRoad(Point p) =0;
      virtual Point placeSecondSettlement() =0;
      virtual Point placeSecondRoad(Point p) =0;
      virtual void discard(int turn) =0;
      virtual int takeTurn(vector<DoublePoint> roadPoints, vector<Point> settlePoints, int playerResources[][5], int playerNumPoints[], int turn) =0;
};
#endif
