#include "Simulation.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
template<typename T, int size>
int length(T(&)[size]){return size;}
const int NUM_PLAYOUTS=3;
const int MAX_TURNS=100;
Simulation::Simulation(Board b, int playerResources[][5], int playerNumPoints[], int tempPlayerToMove){
	board=b;
	board.setVisible(false);
	for(int i=0; i<length(resources); i++)
		for(int j=0; j<length(resources[i]); j++)
			resources[i][j]=playerResources[i][j];
	for(int i=0; i<length(numPoints); i++)
		numPoints[i]=playerNumPoints[i];
	playerToMove=tempPlayerToMove;

	players[0]=new RandComp(board, 1, false);
	players[1]=new RandComp(board, 2, false);
	players[2]=new RandComp(board, 3, false);
	players[3]=new RandComp(board, 4, false);
	numPlayers=length(players);
	turn=0;
}
int Simulation::runPlayouts(int montePlayer, int startTurn){
	Board startBoard = board;
	int value=0;
	int unfinishedGames=0;

	cout<<"About to run "<<NUM_PLAYOUTS<<" simulations"<<endl;
	// This is the code I would like to run in parallel
	for(unsigned int i=0;i<NUM_PLAYOUTS;i++){
		int winner=-1;
		turn=startTurn;
		board=startBoard;
		board.resetDice();
		for(int j=0;j<numPlayers;j++){
			players[j]->resetData();
			players[j]->setResources(resources[j]);
			players[j]->setPoints(numPoints[j]);
		}
		vector<Settlement> newSettlements=board.getSettlements();
		vector<City> newCities=board.getCities();
		vector<Road> newRoads=board.getRoads();
		//cout<<newSettlements.size()<<" "<<newCities.size()<<" "<<newRoads.size()<<endl;
		int settlePlayer, cityPlayer, roadPlayer;
		for(unsigned int j=0; j<newSettlements.size(); j++){
			settlePlayer=newSettlements.at(j).getPlayerNum();
			players[settlePlayer]->addSettlement(newSettlements.at(j).getLocation(), settlePlayer);
		}
		for(unsigned int j=0; j<newCities.size(); j++){
			cityPlayer=newCities.at(j).getPlayerNum();
			players[cityPlayer]->addCity(newCities.at(j).getLocation(), cityPlayer, false);
		}
		for(unsigned int j=0; j<newRoads.size(); j++){
			roadPlayer=newRoads.at(j).getPlayerNum();
			players[roadPlayer]->addRoad(newRoads.at(j).getLocation1(), newRoads.at(j).getLocation2(), roadPlayer);
		}
//		for(int i=0; i<numPlayers; i++){
//			cout<<"For player "<<i+1<<endl;
//			cout<<"Initial number of settlements "<<players[i]->getNumSettlements()<<endl;
//			cout<<"Initial number of cities "<<players[i]->getNumCities()<<endl;
//			cout<<"Initial number of roads "<<players[i]->getNumRoads()<<endl;
//		}
		while(winner==-1)
			winner=takeTurn();
		if(winner==montePlayer)
			value++;
		else if(winner==0)
			unfinishedGames++;
	}
	cout<<"The number of wins by player "<<montePlayer<<" was "<<value<<endl;
	if(unfinishedGames>0)
		cout<<unfinishedGames<<" games were not won within "<<MAX_TURNS<<" turns by any player"<<endl;
	return value;
}
void Simulation::initPlacement()
{
	 Point p1,p2;
	 numPlayers=length(players);
     for(int i=0; i<numPlayers; i++){
		  players[i]->resetBoard(board);
          playerToMove=i+1;
          p1=players[i]->placeFirstSettlement();
          players[i]->addSettlement(p1,i);
          board.addSettlement(p1,i);
//		  cout<<"Player "<<playerToMove<<" placed its first settlement at "
//			  <<p1.getX()<<" "<<p1.getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(p1);
//		  cout<<"Neighbors of player "<<playerToMove<<"'s first settlement are:";
//		  for(unsigned int j=0;j<adjacentHexes.size();j++)
//			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
//		  cout<<endl;
          string newPortType=board.getPortType(p1);
          if(newPortType!=""){
				players[i]->gainPortPower(newPortType);
//				cout<<"Player "<<playerToMove<<" just acquired a "
 //                   <<newPortType<<" port!"<<endl;
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
//		  cout<<"Player "<<playerToMove<<" placed its second settlement at "
 //             <<p1.getX()<< " "<<p1.getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(p1);
		  //cout<<"Neighbors of player "<<playerToMove<<"'s second settlement are:";
//		  for(unsigned int j=0;j<adjacentHexes.size();j++)
//			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
//		  cout<<endl;
          string newPortType=board.getPortType(p1);
		  if(newPortType!=""){
		       players[numPlayers-i-1]->gainPortPower(newPortType);
//		       cout<<"Player "<<playerToMove<<" just acquired a "
//                   <<newPortType<<" port!"<<endl;
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

void Simulation::collectResources(){
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
}

int Simulation::takeTurn(){
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
//		  cout<<"Player "<<playerToMove<<" now has "
//			  << players[playerToMove-1]->getPoints()<<" points on turn "<<turn
//			  <<" after placing a city at "<<cityPoints.at(i).getX()
//			  <<" "<<cityPoints.at(i).getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(cityPoints.at(i));
//		  for(unsigned int j=0;j<adjacentHexes.size();j++)
//			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
//		  cout<<endl;
          board.addCity(cityPoints.at(i),playerToMove-1);
     }

     settlePoints=players[playerToMove-1]->getSettlePoints();
     for(unsigned int i=0; i<settlePoints.size(); i++){
//		  cout<<"Player "<<playerToMove<<" now has "
//			  << players[playerToMove-1]->getPoints()<<" points on turn "<<turn
//			  <<" after placing a settlement at "<<settlePoints.at(i).getX()
//			  <<" "<<settlePoints.at(i).getY()<<" near";
		  vector<Hexagon> adjacentHexes=board.getAdjacentHexes(settlePoints.at(i));
//		  for(unsigned int j=0;j<adjacentHexes.size();j++)
//			  cout<<" "<<adjacentHexes.at(j).getNumber()<<" "<<adjacentHexes.at(j).getType();
//		  cout<<endl;
          board.addSettlement(settlePoints.at(i),playerToMove-1);
          string newPortType=board.getPortType(settlePoints.at(i));
          if(newPortType!=""){
			  players[playerToMove-1]->gainPortPower(newPortType);
//			  cout<<"Player "<<playerToMove<<" just acquired a "
//				  <<newPortType+ " port on turn "<<turn<<"!"<<endl;
          }
     }

     roadPoints=players[playerToMove-1]->getRoadPoints();
	 for(unsigned int i=0; i<roadPoints.size(); i++){
          board.addRoad(roadPoints.at(i).getP1(),roadPoints.at(i).getP2(), playerToMove-1);
//		  cout<<"Player "<<playerToMove<<" just placed a road between "<<roadPoints.at(i).getP1().getX()
//			  <<" "<<roadPoints.at(i).getP1().getY()<<" and "<<roadPoints.at(i).getP2().getX()
//			  <<" "<<roadPoints.at(i).getP2().getY()<<" on turn "<<turn<<endl;
	 }
     if(playerToMove==numPlayers){
          playerToMove=1;
          turn++;
     }
     else
          playerToMove++;
	 if(turn>=MAX_TURNS)
//		 cout<<"Maximum number of turns exceeded in playout, returning no winner"<<endl;
		 return 0;
     return winner;
}
