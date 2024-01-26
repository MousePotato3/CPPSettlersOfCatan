#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <stdlib.h>
#include "Board.h"
#include "RandComp.h"
#include "MonteComp.h"
using namespace std;

class Game{
      Board board;
      Player* players[4];
      vector<string> playerColors;
      vector<Hexagon> neighbors;
      int numPlayers;
      int maxResources;
      int playerToMove;
      int pointsToWin;
  public:
      Game();
      void play();
      void initPlacement();
      void collectResources();
      int takeTurn();
};
#endif
