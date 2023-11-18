#include "Game.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
template<typename T, int size>
int length(T(&)[size]){return size;}
Game::Game()
{
	 players[0]=new MonteComp(board, 1, true);
	 players[1]=new MonteComp(board, 2, true);
	 players[2]=new RandComp(board, 3, true);
	 players[3]=new RandComp(board, 4, true);
     turn=0;
}
void Game::play()
{
     int winner=-1;
     board.initTiles();
     board.initPorts();
     initPlacement();
     while(winner==-1){
          winner=takeTurn();
     }
	 cout<<"Player "<<winner<<" won with "<<players[winner-1]->getPoints()<<" points at the end of turn "<<turn<<"!"<<endl;
}
void Game::initPlacement()
{
	 Point p1,p2;
	 numPlayers=length(players);
     for(int i=0; i<numPlayers; i++){
		  players[i]->resetBoard(board);
          playerToMove=i+1;
          p1=players[i]->placeFirstSettlement();
          players[i]->addSettlement(p1,i);
          board.addSettlement(p1,i);
          cout<<"Player "<<playerToMove<<" placed its first settlement at "
              <<p1.getX()<<" "<<p1.getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(p1);
		  //cout<<"Neighbors of player "<<playerToMove<<"'s first settlement are:";
		  for(unsigned int j=0;j<adjacentHexes.size();j++)
			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
		  cout<<endl;
          string newPortType=board.getPortType(p1);
          if(newPortType!=""){
				players[i]->gainPortPower(newPortType);
				cout<<"Player "<<playerToMove<<" just acquired a "
                    <<newPortType<<" port!"<<endl;
          }
		  //players[i]->resetBoard(board);
          p2=players[i]->placeFirstRoad(p1);
          players[i]->addRoad(p1,p2,i);
          board.addRoad(p1,p2,i);
     }
		
     for(int i=0; i<numPlayers; i++){
		  players[numPlayers-i-1]->resetBoard(board);
          playerToMove=numPlayers-i;
		  p1=players[numPlayers-i-1]->placeSecondSettlement();
		  players[numPlayers-i-1]->addSettlement(p1,numPlayers-i-1);
		  board.addSettlement(p1,numPlayers-i-1);
		  cout<<"Player "<<playerToMove<<" placed its second settlement at "
              <<p1.getX()<< " "<<p1.getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(p1);
		  //cout<<"Neighbors of player "<<playerToMove<<"'s second settlement are:";
		  for(unsigned int j=0;j<adjacentHexes.size();j++)
			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
		  cout<<endl;
          string newPortType=board.getPortType(p1);
		  if(newPortType!=""){
		       players[numPlayers-i-1]->gainPortPower(newPortType);
		       cout<<"Player "<<playerToMove<<" just acquired a "
                   <<newPortType<<" port!"<<endl;
          }
		  //players[numPlayers-i-1]->resetBoard(board);
		  p2=players[numPlayers-i-1]->placeSecondRoad(p1);
		  neighbors=board.getAdjacentHexes(p1);
		  for(unsigned int j=0; j<neighbors.size(); j++){
               players[numPlayers-i-1]->addResource(neighbors[j].getType());
          }
		  players[i]->addRoad(p1, p2, numPlayers-i-1);
		  board.addRoad(p1, p2, numPlayers-i-1);
     }
}

void Game::collectResources(){
     // Eventually players should have an option to play soldier cards before rolling dice
     int diceroll=board.rollDice();
	 //cout<<diceroll<<" rolled on turn "<<turn<<endl;

     if(diceroll==7){
          // Discard
          for(int i=0; i<length(players); i++)
			  if(players[i]->getTotalResources()>7)
                    players[i]->discard(turn);
          // Move robber
          int maxPoints=0;
          int playerToRob=-1;
          for(int i=0; i<length(players); i++)
               if(i!=(playerToMove-1) && players[i]->getPoints()>maxPoints){
                    maxPoints=players[i]->getPoints();
					playerToRob=i;
               }	
          Point robberLoc=board.getPointToBlock(playerToRob, playerToMove-1);
          board.setRobberLocation(robberLoc);

          int resourceNum=players[playerToRob]->getResourceToSteal();
          if(resourceNum!=-1){
               players[playerToMove-1]->gainResource(resourceNum);
               players[playerToRob]->loseResource(resourceNum);
          }
     }
     else{
          for(int i=0; i<numPlayers; i++){
               for(unsigned int j=0; j<players[i]->getSettlements().size(); j++){
                    neighbors=board.getAdjacentHexes(players[i]->getSettlements().at(j).getLocation());
					for(unsigned int k=0; k<neighbors.size(); k++){
						 if(neighbors.at(k).getNumber()==diceroll)
                              players[i]->addResource(neighbors.at(k).getType());
					}
               }
               for(unsigned int j=0; j<players[i]->getCities().size(); j++){
					neighbors=board.getAdjacentHexes(players[i]->getCities().at(j).getLocation());
					for(unsigned int k=0; k<neighbors.size(); k++){
						 if(neighbors.at(k).getNumber()==diceroll
							&& !(neighbors.at(k).getLocation().getX()==board.getRobberLocation().getX()
							&& neighbors.at(k).getLocation().getY()==board.getRobberLocation().getY())){						
							players[i]->addResource(neighbors.at(k).getType());
							players[i]->addResource(neighbors.at(k).getType());
						 }						
					}
               }
          }
     }
//	 for(int i=0;i<length(players); i++)
//		 players[i]->printResources(turn);
}

int Game::takeTurn(){
     vector<Point> cityPoints;
     vector<DoublePoint> possibleRoadPoints;
     vector<DoublePoint> roadPoints;
     vector<Point> possibleSettlePoints;
     vector<Point> settlePoints;
	 int playerResources[4][5];
	 int playerNumPoints[4];

     collectResources();
	 players[playerToMove-1]->resetBoard(board);
	 for(int i=0;i<numPlayers;i++)
	      players[i]->getResources(playerResources[i]);
	 for(int i=0;i<numPlayers;i++)
		  playerNumPoints[i]=players[i]->getPoints();

     possibleRoadPoints=board.getRoadLocations(playerToMove-1);
     possibleSettlePoints=board.getSettleLocations(playerToMove-1);
	 int winner=players[playerToMove-1]->takeTurn(possibleRoadPoints,possibleSettlePoints,playerResources,playerNumPoints,turn);
		
     cityPoints=players[playerToMove-1]->getCityPoints();
     for(unsigned int i=0; i<cityPoints.size(); i++){
          cout<<"Player "<<playerToMove<<" now has "
			  << players[playerToMove-1]->getPoints()<<" points on turn "<<turn
              <<" after placing a city at "<<cityPoints.at(i).getX()
              <<" "<<cityPoints.at(i).getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(cityPoints.at(i));
		  for(unsigned int j=0;j<adjacentHexes.size();j++)
			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
		  cout<<endl;
          board.addCity(cityPoints.at(i),playerToMove-1);
     }

     settlePoints=players[playerToMove-1]->getSettlePoints();
     for(unsigned int i=0; i<settlePoints.size(); i++){
          cout<<"Player "<<playerToMove<<" now has "
			  << players[playerToMove-1]->getPoints()<<" points on turn "<<turn
              <<" after placing a settlement at "<<settlePoints.at(i).getX()
              <<" "<<settlePoints.at(i).getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(settlePoints.at(i));
		  for(unsigned int j=0;j<adjacentHexes.size();j++)
			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
		  cout<<endl;
          board.addSettlement(settlePoints.at(i),playerToMove-1);
          string newPortType=board.getPortType(settlePoints.at(i));
          if(newPortType!=""){
               players[playerToMove-1]->gainPortPower(newPortType);
               cout<<"Player "<<playerToMove<<" just acquired a "
                   <<newPortType+ " port on turn "<<turn<<"!"<<endl;
          }
     }

     roadPoints=players[playerToMove-1]->getRoadPoints();
	 for(unsigned int i=0; i<roadPoints.size(); i++){
          board.addRoad(roadPoints.at(i).getP1(),roadPoints.at(i).getP2(), playerToMove-1);
		  cout<<"Player "<<playerToMove<<" just placed a road between "<<roadPoints.at(i).getP1().getX()
			  <<" "<<roadPoints.at(i).getP1().getY()<<" and "<<roadPoints.at(i).getP2().getX()
			  <<" "<<roadPoints.at(i).getP2().getY()<<" on turn "<<turn<<endl;
	 }
     if(playerToMove==numPlayers){
          playerToMove=1;
          turn++;
     }
     else
          playerToMove++;
     return winner;
}
