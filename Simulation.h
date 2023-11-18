#ifndef SIMULATION_H
#define SIMULATION_H
#include "Board.h"
#include "RandComp.h"
class Simulation{
      Board board;
      Player* players[4];
      vector<Hexagon> neighbors;
	  int resources[4][5];
	  int numPoints[4];
      int numPlayers;
      int playerToMove;
      int turn;
  public:
	  Simulation(Board b, int playerResources[][5], int playerNumPoints[], int tempPlayerToMove);
	  Simulation();
	  int runPlayouts(int montePlayer, int startTurn);
      void initPlacement();
      void collectResources();
      int takeTurn();
};
#endif
