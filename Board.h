#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include "Hexagon.h"
#include "Port.h"
#include "Settlement.h"
#include "City.h"
#include "Road.h"
#include "Dice.h"
using namespace std;

class Board{
	static const int hexNumbers[18];
	vector<Port> ports;
	vector<Settlement> settlements;
	vector<City> cities;
	vector<Road> roads;
	vector<int> numResources;
	vector<int> playerScores;
	vector<Hexagon> tiles;
	Point hexCenters[19];
	Point portTextLocations[9];
	Point portPoint1Locations[9];
	Point portPoint2Locations[9];
	Point robberLocation;
	vector<Point> tempIntersections;
	vector<Point> hexIntersections;
	Dice d;
	bool isVisible;
	int centerX, centerY, width, height, smallWidth, numPlayers, turnNumber, winner;

public:
	Board();
	vector<Settlement> getSettlements() const;
	vector<City> getCities() const;
	vector<Road> getRoads() const;
	vector<int> getNumResources() const;
	vector<int> getPlayerScores() const;
	Point getRobberLocation() const;
	int getTurnNumber() const;
	int getWinner() const;
	int getNumResources(int n) const;
	int getPlayerScore(int n) const;
	Hexagon getHexagon(int i) const;
	Port getPort(int i) const;
	vector<Point> getHexIntersections() const;
	void setVisible(const bool b);
	void setNumPlayers(const int p);
	void setRobberLocation(const Point& p);
	void setTurnNumber(const int t);
	void setWinner(const int w);
	void setNumResources(const int n, const int r);
	void setPlayerScore(const int n, const int s);
	void resetDice();
	int rollDice();
	void gainResource(int i);
	void loseResource(int i);
	void addSettlement(Point p, int n, bool initialPlacement = false);
	void addCity(Point p, int n, bool initialPlacement = false);
	void addRoad(Point p1, Point p2, int n, bool initialPlacement = false);
	void initTiles();
	void initPorts();
	int findHexIndex(Point p);
	int findSettlementIndex(Point p, int n = -1);
	int findCityIndex(Point p, int n = -1);
	int findRoadIndex(Point p1, Point p2, int n = -1);
	bool isAdjacent(Point p1, Point p2);
	string getPortType(Point p);
	vector<Point> getAdjacentIntersections(Point p);
	vector<Hexagon> getAdjacentHexes(Point p);
	bool isConnected(Point p, int n);
	bool legalPlacement(Point p);
	vector<Point> getPossibleCityLocations(int n);
	vector<Point> getPossibleSettlementLocations(int n);
	vector<DoublePoint> getPossibleRoadLocations(int n);
};
#endif
