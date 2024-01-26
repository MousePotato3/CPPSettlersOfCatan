#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "Board.h"
#include "Point.h"
using namespace std;

class Player{
  protected:
	  Board currentBoard;
	  string color;
	  string playerType;
	  int resources[5];
	  int tempResources[5];
	  int tradeRates[5];
	  int tempTradeRates[5];
	  int resourcePoints[5];
	  int playerNum;
	  int numPlayers;
	  int numCities;
	  int numSettlements;
	  int numRoads;
	  int score;
	  bool isVisible;
  public:
	  Player(int n, string c, string t, int p, bool v);
//	  Player();
	  int getScore() const;
	  int getNumCities() const;
	  int getNumSettlements() const;
	  int getNumRoads() const;
	  void getResources(int resourcesToReplace[]) const;
	  void setResources(const int newResources[]);
	  void setScore(const int s);
	  void setNumCities(const int c);
	  void setNumSettlements(const int s);
	  void setNumRoads(const int r);
	  void setVisible(const bool v);
      void resetBoard(const Board& b);
	  void resetData();
      void gainResource(int i);
      void loseResource(int i);
	  string getResourceType(int i);
	  int getResourceIndex(string t);
	  void addResource(string t);
	  void portResource(int o, int n);
	  void tempPortResource(int o, int n);
      int getTotalResources();
	  void updateResourcePoints(Point p);
      void gainPortPower(string t);
	  void tempGainPortPower(string t);
      int getRandomResource();
	  void printResources() const;

      virtual Point chooseInitialSettlementLocation(Board b) = 0;
      virtual Point chooseInitialRoadLocation(Point p) = 0;
      virtual void discard() = 0;
	  virtual int getPlayerToRob() = 0;
	  virtual Point getPointToBlock(int playerToRob) = 0;
      virtual Board takeTurn(Board b) =0;
};
#endif
