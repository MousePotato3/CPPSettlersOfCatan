#ifndef BOARD_H
#define BOARD_H
#include "Hexagon.h"
#include "Port.h"
#include "Settlement.h"
#include "City.h"
#include "Road.h"
#include "Dice.h"
#include <string>
#include <vector>

class Board{
	vector<Hexagon> myTiles;
	static const int hexNumbers[18];
	vector<Port> myPorts;
	vector<Settlement> mySettlements;
	vector<City> myCities;
	vector<Road> myRoads;
	Point hexCenters[19];
	Point portTextLocations[9];
	Point portPoint1Locations[9];
	Point portPoint2Locations[9];
	Point robberLocation;
	vector<Point> tempIntersections;
	vector<Point> hexIntersections;
	Dice d;
	bool isVisible;
	int centerX, centerY, roll;

public:
	Board();
	vector<Settlement> getSettlements();
	vector<City> getCities();
	vector<Road> getRoads();
	Hexagon getHexagon(int i);
	Port getPort(int i);
	string getPortType(Point p);
	vector<Point> getHexIntersections();
	Point getRobberLocation();
	int getTotalSettlements();
	void setVisible(bool b);
	void setRobberLocation(Point p);
	void resetDice();
	int rollDice();
	void addSettlement(Point p, int n);
	void addCity(Point p, int n);
	void addRoad(Point p1, Point p2, int n);
	int findHexIndex(Point p);
	int findSettlementIndex(Point p);
	int findCityIndex(Point p);
	int findRoadIndex(Point p1, Point p2);
	void initTiles();
	void initPorts();
	bool isAdjacent(Point p1, Point p2);
	vector<Point> getAdjacentIntersections(Point p);
	vector<Hexagon> getAdjacentHexes(Point p);
	int getHexValue(Point p);
	bool hasNoNeighbors(Point p);
	vector<Point> getSettleLocations(int n);
	vector<DoublePoint> getRoadLocations(int n);
	Point getPointToBlock(int playerToRob, int robbingPlayer);
};
#endif
