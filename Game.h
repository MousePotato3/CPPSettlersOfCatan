#ifndef GAME_H
#define GAME_H
#include "Board.h"
#include "RandComp.h"
#include "MonteComp.h"
class Game{
      Board board;
      Player* players[4];
      vector<Hexagon> neighbors;
      int numPlayers;
      int playerToMove;
      int turn;
  public:
      Game();
      void play();
      void initPlacement();
      void collectResources();
      int takeTurn();
};
#endif
