#include <time.h>
#include "MonteComp.h"
#include "Simulation.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

template<typename T, int size>
int length(T(&)[size]){return size;}

MonteComp::MonteComp(Board startBoard, int n, bool b):Player(){
     srand ( (unsigned int)time(NULL) );
     board=startBoard;
     playerNum=n;
	 setVisible(b);
}
MonteComp::MonteComp():Player(){cerr<<"Inside empty MonteComp default constructor"<<endl;}
Point MonteComp::placeFirstSettlement(){
	vector<Hexagon> adjacentHexes;
	int hexValue;
	int maxValue=-1;
	int maxIndex=-1;

	//cout<<"Entering PlaceFirstSettlement"<<endl;
	hexIntersections=board.getHexIntersections();
	
	for(unsigned int i=0; i<hexIntersections.size(); i++){
		hexValue=0;
		adjacentHexes=board.getAdjacentHexes(hexIntersections.at(i));
		//cout<<"Number of adjacent hexes: "<< adjacentHexes.size()<<endl;
		for(unsigned int j=0; j<adjacentHexes.size(); j++)
			hexValue+=adjacentHexes[j].getValue();
		if(hexValue>maxValue)
			if(board.hasNoNeighbors(hexIntersections.at(i))){
				maxValue=hexValue;
				maxIndex=i;
			}
			else{
				//cout<<"Found a point where a settlement could not be placed"<<endl;
			}
			//cout << "Value of hex at " << hexIntersections.at(i).getX()<<" "<<hexIntersections.at(i).getY()<< ": "<< hexValue << endl;
	}
	points++;
	return hexIntersections.at(maxIndex);
}

Point MonteComp::placeFirstRoad(Point p){
	vector<Point> possibleRoadPoints=board.getAdjacentIntersections(p);
	int roadNum=rand()%possibleRoadPoints.size();
	return possibleRoadPoints.at(roadNum);
}

Point MonteComp::placeSecondSettlement(){
	return placeFirstSettlement();
}

Point MonteComp::placeSecondRoad(Point p){
	return placeFirstRoad(p);
}
void MonteComp::discard(int turn){
	int numberToDiscard=getTotalResources()/2;
	cout<<"Player "<<playerNum<<" discarded "<<numberToDiscard<<" resources on turn "<<turn<<endl;
	for(int i=0; i<numberToDiscard; i++){
		int maxIndex=0;
		int maxResources=resources[0];
		for(int j=1; j<length(resources);j++){
			if(resources[j]>=maxResources){
				maxIndex=j;
				maxResources=resources[j];
			}
		}
		resources[maxIndex]--;
	}
}

vector<int> MonteComp::canPlaceCity(){
     vector<int> resourcesToTrade;
     if(cities.size()>=4){
		  resourcesToTrade.clear();
          resourcesToTrade.push_back(-1);
          return resourcesToTrade;
     }
     int tempResources[5];
     for(int i=0;i<length(resources); i++){
          tempResources[i]=resources[i];
     }
     int maxRemainingResources;
     do{
          if(tempResources[0]>=3 && tempResources[1]>=2)
               return resourcesToTrade;
          int maxIndex=0;
          maxRemainingResources=tempResources[0]-3-tradeRates[0];
          if(tempResources[1]-2-tradeRates[1] >= maxRemainingResources){
               maxIndex=1;
               maxRemainingResources=tempResources[1]-2-tradeRates[1];
          }
          for(int i=2; i<length(tempResources); i++){
               if(tempResources[i]-tradeRates[i] >= maxRemainingResources){
                    maxIndex=i;
					maxRemainingResources=tempResources[i]-tradeRates[i];
               }
          }
          if(maxRemainingResources>=0){
               if(tempResources[0]<3)
                    tempResources[0]++;
               else
					tempResources[1]++;
               resourcesToTrade.push_back(maxIndex);
               tempResources[maxIndex]-=tradeRates[maxIndex];
          }
     } while(maxRemainingResources>=0);
     // If search fails, set the last resource to trade to -1 as a flag
	 resourcesToTrade.clear();
     resourcesToTrade.push_back(-1);
     return resourcesToTrade;
}

vector<int> MonteComp::canPlaceSettlement(){
     vector<int> resourcesToTrade;
     if(settlements.size()>=5){
//		  cout<<"Player "<<playerNum<<" has "<<settlements.size()
//			  <<" settlements and cannot build one more"<<endl;
		  resourcesToTrade.clear();
          resourcesToTrade.push_back(-1);
          return resourcesToTrade;
     }
     int tempResources[5];
     for(int i=0;i<length(resources); i++){
          tempResources[i]=resources[i];
     }
     int maxRemainingResources;
     do{
          if(tempResources[1]>0 && tempResources[2]>0 && tempResources[3]>0 && tempResources[4]>0)
               return resourcesToTrade;
          int maxIndex=0;
          maxRemainingResources=tempResources[0]-tradeRates[0];
          for(int i=1; i<length(tempResources); i++){
               if(tempResources[i]-1-tradeRates[i]>=maxRemainingResources){
                    maxIndex=i;
					maxRemainingResources=tempResources[i]-1-tradeRates[i];
               }
          }
          if(maxRemainingResources>=0){
               if(tempResources[1]==0)
					tempResources[1]++;
               else if(tempResources[2]==0)
					tempResources[2]++;
               else if(tempResources[3]==0)
					tempResources[3]++;
               else if(tempResources[4]==0)
                    tempResources[4]++;
               resourcesToTrade.push_back(maxIndex);
               tempResources[maxIndex]-=tradeRates[maxIndex];
          }
     } while(maxRemainingResources>=0);
	 resourcesToTrade.clear();
     resourcesToTrade.push_back(-1);
     return resourcesToTrade;
}	

vector<int> MonteComp::canPlaceRoad(){
     vector<int> resourcesToTrade;
     if(roads.size()>=15){
		  resourcesToTrade.clear();
          resourcesToTrade.push_back(-1);
          return resourcesToTrade;
     }
     int tempResources[5];
     for(int i=0;i<length(resources); i++){
          tempResources[i]=resources[i];
     }
     int maxRemainingResources;
     do{
          if(tempResources[3]>0 && tempResources[4]>0)
               return resourcesToTrade;
          int maxIndex=0;
          maxRemainingResources=tempResources[0]-tradeRates[0];
          for(int i=1; i<length(tempResources); i++){
               if(tempResources[i]-1-tradeRates[i]>=maxRemainingResources){
                    maxIndex=i;
					maxRemainingResources=tempResources[i]-1-tradeRates[i];
               }
          }
          if(maxRemainingResources>=0){
               if(tempResources[3]==0)
                    tempResources[3]++;
               else if(tempResources[4]==0)
					tempResources[4]++;
               resourcesToTrade.push_back(maxIndex);
               tempResources[maxIndex]-=tradeRates[maxIndex];
          }
     } while(maxRemainingResources>=0);
	 resourcesToTrade.clear();
     resourcesToTrade.push_back(-1);
     return resourcesToTrade;
}

int MonteComp::placeCity(int playerResources[][5], int playerNumPoints[], int turn){
	vector<int> resourcesToTrade=canPlaceCity();
	//cout<<"City number of resources to trade: "<<resourcesToTrade.size()<<endl;
	if(settlements.size()==0||(resourcesToTrade.size()>0 && resourcesToTrade.at(0)==-1))
		return -1;

	//Default option is not to place a city
	int maxValue=runSimulation(board,playerResources,playerNumPoints,turn);
	int maxIndex=-1;

	// First make the necessary port trades to place the city and adjust temporary resources
	for(unsigned int i=0; i<resourcesToTrade.size(); i++){
		int resourceReceived;
		if(playerResources[playerNum-1][0]<3&&resourcesToTrade.at(i)!=0)
			resourceReceived=0;
		else
			resourceReceived=1;
		playerResources[playerNum-1][resourceReceived]++;
		playerResources[playerNum-1][resourcesToTrade.at(i)]-=tradeRates[resourcesToTrade.at(i)];
	}
	playerResources[playerNum-1][0]-=3;
	playerResources[playerNum-1][1]-=2;
	playerNumPoints[playerNum-1]++;

	// Now place the city and run the simulations
	for(unsigned int i=0;i<settlements.size();i++){
		Board newBoard=board;
		newBoard.addCity(settlements[i].getLocation(),playerNum-1);
		int value=runSimulation(newBoard,playerResources,playerNumPoints,turn);
		if(value>=maxValue){
			maxValue=value;
			maxIndex=i;
		}
	}
	if(maxIndex==-1)
		return -1;

	for(unsigned int i=0; i<resourcesToTrade.size(); i++){
		int resourceReceived;
		if(resources[0]<3&&resourcesToTrade.at(i)!=0)
			resourceReceived=0;
		else
			resourceReceived=1;
		resources[resourceReceived]++;
		resources[resourcesToTrade.at(i)]-=tradeRates[resourcesToTrade.at(i)];
		if(isVisible){
			cout<<"Player "<<playerNum<<" just traded "<<tradeRates[resourcesToTrade.at(i)]
				<<" "<<getResourceType(resourcesToTrade.at(i))<<" for 1 "
				<<getResourceType(resourceReceived)<<" from a port"<<endl;
		}
	}

	if(resources[0]>=3 && resources[1]>=2){
		resources[0]-=3;
		resources[1]-=2;
		Point cityLocation=settlements[maxIndex].getLocation();
		addCity(cityLocation,playerNum-1,true);
		newCityPoints.push_back(cityLocation);
		points++;
		return maxIndex;
	}
	else{
		cout<<"Player "<<playerNum<<" messed up its city placement on turn "<<turn<<endl;
	}
	return -1;
}

int MonteComp::placeSettlement(vector<Point> possibleSettlements, int playerResources[][5], int playerNumPoints[], int turn){
	vector<int> resourcesToTrade=canPlaceSettlement();
	//cout<<"Settle number of resources to trade: "<<resourcesToTrade.size()<<endl;
	if(possibleSettlements.size()==0||(resourcesToTrade.size()>0 && resourcesToTrade.at(0)==-1))
		return -1;

	// Default option is not to place a settlement
	int maxValue=runSimulation(board,playerResources,playerNumPoints,turn);
	int maxIndex=-1;

	// First make the necessary port trades to place the settlement and adjust temporary resources
	for(unsigned int j=0; j<resourcesToTrade.size(); j++){
		int resourceReceived;
		if(playerResources[playerNum-1][1]==0&&resourcesToTrade.at(j)!=1)
			resourceReceived=1;
		else if(playerResources[playerNum-1][2]==0&&resourcesToTrade.at(j)!=2)
			resourceReceived=2;
		else if(playerResources[playerNum-1][3]==0&&resourcesToTrade.at(j)!=3)
			resourceReceived=3;
		else
			resourceReceived=4;
		playerResources[playerNum-1][resourceReceived]++;
		playerResources[playerNum-1][resourcesToTrade.at(j)]-=tradeRates[resourcesToTrade.at(j)];
	}
	playerResources[playerNum-1][1]--;
	playerResources[playerNum-1][2]--;
	playerResources[playerNum-1][3]--;
	playerResources[playerNum-1][4]--;
	playerNumPoints[playerNum-1]++;

	// Now place the settlement and run the simulations
	for(unsigned int i=0;i<possibleSettlements.size();i++){
		Board newBoard=board;
		newBoard.addSettlement(possibleSettlements.at(i),playerNum-1);
		int value=runSimulation(newBoard,playerResources,playerNumPoints,turn);
		if(value>=maxValue){
			maxValue=value;
			maxIndex=i;
		}
	}
	if(maxIndex==-1)
		return -1;
	for(unsigned int i=0; i<resourcesToTrade.size(); i++){
		int resourceReceived;
		if(resources[1]==0&&resourcesToTrade.at(i)!=1)
			resourceReceived=1;
		else if(resources[2]==0&&resourcesToTrade.at(i)!=2)
			resourceReceived=2;
		else if(resources[3]==0&&resourcesToTrade.at(i)!=3)
			resourceReceived=3;
		else
			resourceReceived=4;
		resources[resourceReceived]++;
		resources[resourcesToTrade.at(i)]-=tradeRates[resourcesToTrade.at(i)];
		if(isVisible){
			cout<<"Player "<<playerNum<<" just traded "<<tradeRates[resourcesToTrade.at(i)]
				<<" "<<getResourceType(resourcesToTrade.at(i))<<" for 1 "
				<<getResourceType(resourceReceived)<<" from a port"<<endl;
		}
	}
	if(resources[1]>0 && resources[2]>0 && resources[3]>0 && resources[4]>0){
		resources[1]--;
		resources[2]--;
		resources[3]--;
		resources[4]--;
		int settleIndex=rand()%possibleSettlements.size();
		Point settleLocation=possibleSettlements.at(settleIndex);
		addSettlement(settleLocation,playerNum-1);
		newSettlePoints.push_back(settleLocation);
		points++;
		return settleIndex;
	}
	else{
		cout<<"Player "<<playerNum<<" messed up its settlement placement on turn "<<turn<<endl;
	}
	return -1;
}

int MonteComp::placeRoad(vector<DoublePoint> possibleRoads, int playerResources[][5], int playerNumPoints[], int turn){
	vector<int> resourcesToTrade=canPlaceRoad();
	//cout<<"Road number of resources to trade: "<<resourcesToTrade.size()<<endl;
	if(possibleRoads.size()==0||(resourcesToTrade.size()>0 && resourcesToTrade.at(0)==-1))
		return -1;

	// Default option is not to place a road
	int maxValue=runSimulation(board,playerResources,playerNumPoints,turn);
	int maxIndex=-1;

	// First make the necessary port trades to place the road and adjust temporary resources
	for(unsigned int j=0; j<resourcesToTrade.size(); j++){
		int resourceReceived;
		if(playerResources[playerNum-1][3]==0&&resourcesToTrade.at(j)!=3)
			resourceReceived=3;
		else
			resourceReceived=4;
		playerResources[playerNum-1][resourceReceived]++;
		playerResources[playerNum-1][resourcesToTrade.at(j)]-=tradeRates[resourcesToTrade.at(j)];
	}
	playerResources[playerNum-1][3]--;
	playerResources[playerNum-1][4]--;

	// Now place the road and run the simulations
	for(unsigned int i=0;i<possibleRoads.size();i++){
		Board newBoard=board;
		newBoard.addRoad(possibleRoads.at(i).getP1(),possibleRoads.at(i).getP2(),playerNum-1);
		int value=runSimulation(newBoard,playerResources,playerNumPoints,turn);
		if(value>=maxValue){
			maxValue=value;
			maxIndex=i;
		}
	}
	if(maxIndex==-1)
		return -1;
	for(unsigned int i=0; i<resourcesToTrade.size(); i++){
		int resourceReceived;
		if(resources[3]==0&&resourcesToTrade.at(i)!=3)
			resourceReceived=3;
		else
			resourceReceived=4;
		resources[resourceReceived]++;
		resources[resourcesToTrade.at(i)]-=tradeRates[resourcesToTrade.at(i)];
		if(isVisible){
			cout<<"Player "<<playerNum<<" just traded "<<tradeRates[resourcesToTrade.at(i)]
				<<" "<<getResourceType(resourcesToTrade.at(i))<<" for 1 "
				<<getResourceType(resourceReceived)<<" from a port"<<endl;
		}
	}
	if(resources[3]>0 && resources[4]>0 && possibleRoads.size()>0){
		resources[3]--;
		resources[4]--;
		addRoad(possibleRoads.at(maxIndex).getP1(),possibleRoads.at(maxIndex).getP2(),playerNum-1);
		newRoadPoints.push_back(possibleRoads.at(maxIndex));
		return maxIndex;
	}
	else{
		cout<<"Player "<<playerNum<<" messed up its road placement on turn "<<turn<<endl;
	}
	return -1;
}
     
int MonteComp::takeTurn(vector<DoublePoint> roadPoints, vector<Point> settlePoints, int playerResources[][5], int playerNumPoints[], int turn){
	newRoadPoints.clear();
	newSettlePoints.clear();
	newCityPoints.clear();

	int cityIndex,settleIndex,roadIndex;
	do{
		cityIndex=placeCity(playerResources, playerNumPoints, turn);
		settleIndex=placeSettlement(settlePoints, playerResources, playerNumPoints, turn);
		roadIndex=placeRoad(roadPoints, playerResources, playerNumPoints, turn);
		if(settleIndex!=-1){
			settlePoints.at(settleIndex)=settlePoints.back();
			settlePoints.pop_back();
		}
		if(roadIndex!=-1){
			roadPoints.at(roadIndex)=roadPoints.back();
			roadPoints.pop_back();
		}
	} while (cityIndex!=-1 && settleIndex!=-1 && roadIndex!=-1);
	if(points>=10)
		return playerNum;
	return -1;
}

int MonteComp::runSimulation(Board newBoard, int playerResources[][5], int playerNumPoints[], int turn){
	int playerToMove;
	if(playerNum==4)
		playerToMove=1;
	else
		playerToMove=playerNum+1;
	Simulation s=Simulation(newBoard,playerResources,playerNumPoints,playerToMove);
	int value=s.runPlayouts(playerNum,turn);
	return value;
}
