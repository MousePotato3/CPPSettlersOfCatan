#ifndef SIMULATION_H
#define SIMULATION_H
#include <iostream>
#include <stdlib.h>
#include "Board.h"
#include "RandComp.h"
using namespace std;

class Simulation{
      Board board;
      Player* players[4];
      vector<Hexagon> neighbors;
      string actionType;
	  int initResources[4][5];
	  int initScores[4];
      int numPlayers;
      int maxResources;
      int playerToMove;
      int pointsToWin;
  public:
	  Simulation(Board b, string a, int playerResources[], int playerScore, int montePlayer);
	  int runPlayouts(int montePlayer);
      void generateRandomResources(int playerNum, int numResources);
      void initPlacement();
      void collectResources();
      int takeTurn();
};
#endif
