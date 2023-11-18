#include "Board.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
using namespace std;

template<typename T, int size>
int length(T(&)[size]){return size;}

template<typename T>
vector<T> removeDuplicates(vector<T> s){
	bool duplicates = false;
	vector<T> v;
	for (unsigned int i=0; i<s.size(); i++) {
		duplicates = false;
	for (unsigned int j = (i+1); j < s.size(); j++)
		if(s.at(i).equals(s.at(j)))
			duplicates = true;
	if (duplicates == false)
		v.push_back(s.at(i));
	}
	return v;
}

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 32;
const int WIDTH = 75;
const int HEIGHT = 65;
const int SMALLWIDTH = 37;
const int Board::hexNumbers[18]={5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11};

Board::Board()
{
	centerX=SCREEN_WIDTH/2;
	centerY=SCREEN_HEIGHT/2-20;
}
vector<Settlement> Board::getSettlements(){return mySettlements;}
vector<City> Board::getCities(){return myCities;}
vector<Road> Board::getRoads(){return myRoads;}
void Board::setVisible(bool b) {isVisible=b;}
Hexagon Board::getHexagon(int i) {return myTiles[i];}
Port Board::getPort(int i) {return myPorts[i];}
string Board::getPortType(Point p){
	for(unsigned int i=0; i<myPorts.size(); i++){
		if(p.equals(myPorts.at(i).getPortLocations().getP1()) 
			|| p.equals(myPorts.at(i).getPortLocations().getP2()))
			return myPorts.at(i).getType();
	}
	return "";
}
vector<Point> Board::getHexIntersections() {return hexIntersections;}
Point Board::getRobberLocation() {return robberLocation;}
int Board::getTotalSettlements(){return mySettlements.size();}
void Board::setRobberLocation(Point p){
	robberLocation.setX(p.getX());
	robberLocation.setY(p.getY());
}
void Board::resetDice() {d.resetDice();}
int Board::rollDice() {return d.rollDice();}
void Board::addSettlement(Point p, int n) {
	mySettlements.push_back(Settlement(p,n));
}
void Board::addCity(Point p, int n){
	myCities.push_back(City(p,n));
}
void Board::addRoad(Point p1, Point p2, int n){
	if(p1.getX()>p2.getX())
		myRoads.push_back(Road(p2,p1,n));
	else
		myRoads.push_back(Road(p1,p2,n));
}
int Board::findHexIndex(Point p){
	for(unsigned int i=0; i<myTiles.size(); i++)
		if(myTiles.at(i).getLocation().getX()==p.getX()
			&& myTiles.at(i).getLocation().getY()==p.getY())
			return i;
	return -1;
}
int Board::findSettlementIndex(Point p){
	for(unsigned int i=0; i<mySettlements.size(); i++)
		if(p.equals(mySettlements.at(i).getLocation()))
			return i;
	return -1;
}
int Board::findCityIndex(Point p){
	for(unsigned int i=0; i<myCities.size(); i++)
		if(p.equals(myCities.at(i).getLocation()))
			return i;
	return -1;
}
int Board::findRoadIndex(Point p1,Point p2){
	for(unsigned int i=0; i<myRoads.size(); i++){
		if((p1.equals(myRoads.at(i).getLocation1())&&p2.equals(myRoads.at(i).getLocation2()))
			|| (p2.equals(myRoads.at(i).getLocation1())&&p1.equals(myRoads.at(i).getLocation2())))
			return i;
	}
	return -1;
}

void Board::initTiles(){
	int i;
	int counter=0;

	myTiles.push_back(Hexagon("desert"));
	for(i=1; i<4; i++)
		myTiles.push_back(Hexagon("ore"));
	for(i=4; i<7; i++)
		myTiles.push_back(Hexagon("clay"));
	for(i=7; i<11; i++)
		myTiles.push_back(Hexagon("sheep"));
	for(i=11; i<15; i++)
		myTiles.push_back(Hexagon("wheat"));
	for(i=15; i<19; i++)
		myTiles.push_back(Hexagon("wood"));
	random_shuffle ( myTiles.begin(), myTiles.end() );

	// The points are added in a spiral starting at the top 
	// and moving counter-clockwise toward the center
	hexCenters[0]=Point(centerX,centerY-2*HEIGHT*2);
	hexCenters[1]=Point(centerX-(WIDTH+SMALLWIDTH),centerY-3*HEIGHT);
	hexCenters[2]=Point(centerX-(WIDTH*2+SMALLWIDTH*2),centerY-HEIGHT*2);
	hexCenters[3]=Point(centerX-(WIDTH*2+SMALLWIDTH*2),centerY);
	hexCenters[4]=Point(centerX-(WIDTH*2+SMALLWIDTH*2),centerY+HEIGHT*2);
	hexCenters[5]=Point(centerX-(WIDTH+SMALLWIDTH),centerY+3*HEIGHT);
	hexCenters[6]=Point(centerX,centerY+2*HEIGHT*2);
	hexCenters[7]=Point(centerX+(WIDTH+SMALLWIDTH),centerY+3*HEIGHT);
	hexCenters[8]=Point(centerX+(WIDTH*2+SMALLWIDTH*2),centerY+HEIGHT*2);
	hexCenters[9]=Point(centerX+(WIDTH*2+SMALLWIDTH*2),centerY);
	hexCenters[10]=Point(centerX+(WIDTH*2+SMALLWIDTH*2),centerY-HEIGHT*2);
	hexCenters[11]=Point(centerX+(WIDTH+SMALLWIDTH),centerY-3*HEIGHT);
	hexCenters[12]=Point(centerX,centerY-HEIGHT*2);
	hexCenters[13]=Point(centerX-(WIDTH+SMALLWIDTH),centerY-HEIGHT);
	hexCenters[14]=Point(centerX-(WIDTH+SMALLWIDTH),centerY+HEIGHT);
	hexCenters[15]=Point(centerX,centerY+HEIGHT*2);
	hexCenters[16]=Point(centerX+(WIDTH+SMALLWIDTH),centerY+HEIGHT);
	hexCenters[17]=Point(centerX+(WIDTH+SMALLWIDTH),centerY-HEIGHT);
	hexCenters[18]=Point(centerX,centerY);

	for(i=0; i<19; i++){
		myTiles.at(i).setLocation(hexCenters[i]);
		if(myTiles.at(i).getType()!="desert") {
			myTiles.at(i).setNumber(hexNumbers[counter]);
			counter++;
		}
		else{
			myTiles.at(i).setNumber(1);
			robberLocation=myTiles.at(i).getLocation();
		}
	}

	for(i=0; i<length(hexCenters); i++){
		int x=hexCenters[i].getX();
		int y=hexCenters[i].getY();
			
		tempIntersections.push_back(Point(x+WIDTH,y));
		tempIntersections.push_back(Point(x-WIDTH,y));
		tempIntersections.push_back(Point(x+SMALLWIDTH,y+HEIGHT));
		tempIntersections.push_back(Point(x-SMALLWIDTH,y+HEIGHT));
		tempIntersections.push_back(Point(x+SMALLWIDTH,y-HEIGHT));
		tempIntersections.push_back(Point(x-SMALLWIDTH,y-HEIGHT));
	}
	hexIntersections = removeDuplicates(tempIntersections);
}

void Board::initPorts(){
	//cout<<"Inside initPorts"<<endl;
	for(int i=0; i<4; i++)
		myPorts.push_back(Port("general"));
	myPorts.push_back(Port("clay"));
	myPorts.push_back(Port("ore"));
	myPorts.push_back(Port("sheep"));
	myPorts.push_back(Port("wheat"));
	myPorts.push_back(Port("wood"));
	random_shuffle ( myPorts.begin(), myPorts.end() );

	portTextLocations[0]=Point(centerX-SMALLWIDTH+15, centerY+5*HEIGHT+30);
	portPoint1Locations[0]=Point(centerX-SMALLWIDTH,centerY+5*HEIGHT);
	portPoint2Locations[0]=Point(centerX+SMALLWIDTH,centerY+5*HEIGHT);		

	portTextLocations[1]=Point(centerX-(WIDTH*2+SMALLWIDTH*2)+10, centerY+7*HEIGHT/2+15);
	portPoint1Locations[1]=Point(centerX-(WIDTH+SMALLWIDTH*2),centerY+2*HEIGHT*2);
	portPoint2Locations[1]=Point(centerX-(WIDTH*2+SMALLWIDTH),centerY+3*HEIGHT);

	portTextLocations[2]=Point(centerX-3*(WIDTH+SMALLWIDTH)+10, centerY+HEIGHT/2+15);
	portPoint1Locations[2]=Point(centerX-(3*WIDTH+SMALLWIDTH*2),centerY);
	portPoint2Locations[2]=Point(centerX-(WIDTH*2+3*SMALLWIDTH),centerY+HEIGHT);

	portTextLocations[3]=Point(centerX-3*(WIDTH+SMALLWIDTH)+10, centerY-5*HEIGHT/2+5);
	portPoint1Locations[3]=Point(centerX-(3*WIDTH+SMALLWIDTH*2),centerY-HEIGHT*2);
	portPoint2Locations[3]=Point(centerX-(WIDTH*2+3*SMALLWIDTH),centerY-3*HEIGHT);

	portTextLocations[4]=Point(centerX-(WIDTH+SMALLWIDTH*2)+15, centerY-2*HEIGHT*2-5);
	portPoint1Locations[4]=Point(centerX-(WIDTH+SMALLWIDTH*2),centerY-2*HEIGHT*2);
	portPoint2Locations[4]=Point(centerX-WIDTH,centerY-2*HEIGHT*2);

	portTextLocations[5]=Point(centerX+WIDTH+15, centerY-2*HEIGHT*2-5);
	portPoint1Locations[5]=Point(centerX+WIDTH+SMALLWIDTH*2,centerY-2*HEIGHT*2);
	portPoint2Locations[5]=Point(centerX+WIDTH,centerY-2*HEIGHT*2);

	portTextLocations[6]=Point(centerX+(WIDTH*2+3*SMALLWIDTH)+23, centerY-5*HEIGHT/2+5);
	portPoint1Locations[6]=Point(centerX+(3*WIDTH+SMALLWIDTH*2),centerY-HEIGHT*2);
	portPoint2Locations[6]=Point(centerX+(WIDTH*2+3*SMALLWIDTH),centerY-3*HEIGHT);

	portTextLocations[7]=Point(centerX+(WIDTH*2+3*SMALLWIDTH)+23, centerY+HEIGHT/2+15);
	portPoint1Locations[7]=Point(centerX+(3*WIDTH+SMALLWIDTH*2),centerY);
	portPoint2Locations[7]=Point(centerX+(WIDTH*2+3*SMALLWIDTH),centerY+HEIGHT);

	portTextLocations[8]=Point(centerX+(WIDTH+SMALLWIDTH*2)+23, centerY+7*HEIGHT/2+15);
	portPoint1Locations[8]=Point(centerX+(WIDTH+SMALLWIDTH*2),centerY+2*HEIGHT*2);
	portPoint2Locations[8]=Point(centerX+(WIDTH*2+SMALLWIDTH),centerY+3*HEIGHT);
 
	for(unsigned int i=0; i<myPorts.size(); i++){
		myPorts.at(i).setLocation(portTextLocations[i]);
		myPorts.at(i).setPortLocations(DoublePoint(portPoint1Locations[i],portPoint2Locations[i]));
		DoublePoint dp=myPorts.at(i).getPortLocations();
		DoublePoint kp=DoublePoint(portPoint1Locations[i],portPoint2Locations[i]);
	}
}

bool Board::isAdjacent(Point p1, Point p2){
	float distance=sqrt(pow((float)(p1.getX()-p2.getX()),2)+pow((float)(p1.getY()-p2.getY()),2));
	if(abs((int)distance-SMALLWIDTH*2)<2)
		return true;
	return false;
}

vector<Point> Board::getAdjacentIntersections(Point p){
	vector<Point> myPoints;

	for(unsigned int i=0; i<hexIntersections.size(); i++)
		if(isAdjacent(p,hexIntersections.at(i)))
			myPoints.push_back(hexIntersections.at(i));
	return myPoints;
}

vector<Hexagon> Board::getAdjacentHexes(Point p){
	int x=p.getX();
	int y=p.getY();
	vector<Hexagon> myHexes;
	int myInts[6];

	myInts[0]=findHexIndex(Point(x+WIDTH,y));
	myInts[1]=findHexIndex(Point(x-WIDTH,y));
	myInts[2]=findHexIndex(Point(x+SMALLWIDTH,y+HEIGHT));
	myInts[3]=findHexIndex(Point(x-SMALLWIDTH,y+HEIGHT));
	myInts[4]=findHexIndex(Point(x+SMALLWIDTH,y-HEIGHT));
	myInts[5]=findHexIndex(Point(x-SMALLWIDTH,y-HEIGHT));
		
	for(int i=0; i<length(myInts); i++)
		if(myInts[i]!=-1)
			myHexes.push_back(myTiles.at(myInts[i]));
	return myHexes;
}

int Board::getHexValue(Point p){
	vector<Hexagon> adjacentHexes=getAdjacentHexes(p);
	int hexValue=0;
	for(unsigned int i=0; i<adjacentHexes.size(); i++){
		hexValue+=adjacentHexes[i].getValue();
	}
	return hexValue;
}

bool Board::hasNoNeighbors(Point p){
	bool isHexLegal=true;
	if(findSettlementIndex(p)!=-1 || findCityIndex(p)!=-1)
		isHexLegal=false;
	vector<Point> checkPoints=getAdjacentIntersections(p);
	for(unsigned int j=0;j<checkPoints.size();j++)
		if(findSettlementIndex(checkPoints.at(j))!=-1 || findCityIndex(checkPoints.at(j))!=-1)
			isHexLegal=false;
	return isHexLegal;
}

vector<Point> Board::getSettleLocations(int n){
	vector<Point> playerRoadPoints;
	vector<Point> settlePoints;

	for(unsigned int i=0; i<myRoads.size(); i++){
		if(myRoads.at(i).getPlayerNum()==n){
			playerRoadPoints.push_back(myRoads.at(i).getLocation1());
			playerRoadPoints.push_back(myRoads.at(i).getLocation2());
		}
	}
	removeDuplicates(playerRoadPoints);		
		
	for(unsigned int i=0; i<playerRoadPoints.size(); i++)
		if(hasNoNeighbors(playerRoadPoints.at(i)))
			settlePoints.push_back(playerRoadPoints.at(i));
	return settlePoints;
}

vector<DoublePoint> Board::getRoadLocations(int n){
	vector<Point> playerRoadPoints;
	vector<Point> neighbors;
	vector<DoublePoint> roadLocations;
	vector<DoublePoint> bestLocations;
	vector<DoublePoint> okayLocations;
		
	for(unsigned int i=0; i<myRoads.size(); i++){
		if(myRoads.at(i).getPlayerNum()==n){
			playerRoadPoints.push_back(myRoads.at(i).getLocation1());
			playerRoadPoints.push_back(myRoads.at(i).getLocation2());
		}
	}
	removeDuplicates(playerRoadPoints);
		
	for(unsigned int i=0; i<playerRoadPoints.size(); i++){
		// Ignore if there is a different player's settlement or city on the start location of the road
		int index1=findSettlementIndex(playerRoadPoints.at(i));
		int index2=findCityIndex(playerRoadPoints.at(i));
		if((index1!=-1 && mySettlements.at(index1).getPlayerNum()!=n)
			||(index2!=-1 && myCities.at(index2).getPlayerNum()!=n))
			break;
			
		neighbors=getAdjacentIntersections(playerRoadPoints.at(i));
		for(unsigned int j=0; j<neighbors.size(); j++)
			if(findRoadIndex(playerRoadPoints.at(i),neighbors.at(j))==-1)
				roadLocations.push_back(*(new DoublePoint(playerRoadPoints.at(i),neighbors.at(j))));
	}
	removeDuplicates(roadLocations);

	for(unsigned int i=0; i<roadLocations.size(); i++)
		if(hasNoNeighbors(roadLocations.at(i).getP2()))
			bestLocations.push_back(roadLocations.at(i));
	if(bestLocations.size()>0)
		return bestLocations;
	else{
		for(unsigned int i=0; i<roadLocations.size(); i++){
			vector<Point> checkPoints=getAdjacentIntersections(roadLocations.at(i).getP2());
			for(unsigned int j=0; j<checkPoints.size(); j++){
				if(hasNoNeighbors(checkPoints.at(j))){
					okayLocations.push_back(roadLocations.at(i));
					break;
				}
			}
		}
		if(okayLocations.size()>0)
			return okayLocations;
	}
	return roadLocations;
}

Point Board::getPointToBlock(int playerToRob, int robbingPlayer){
	vector<Hexagon> neighbors;
	vector<Point> checkPoints;
	vector<Hexagon> tempHexes;
	vector<Hexagon> possibleBlockedHexes;
	int owner,value,maxValue,maxIndex,index1,index2;

	for(int unsigned i=0; i<mySettlements.size(); i++){
		if(mySettlements.at(i).getPlayerNum()!=playerToRob)
			continue;
		neighbors=getAdjacentHexes(mySettlements.at(i).getLocation());
		for(unsigned int j=0; j<neighbors.size(); j++)
			tempHexes.push_back(neighbors[j]);
	}
	for(unsigned int i=0; i<myCities.size(); i++){
		if(myCities.at(i).getPlayerNum()!=playerToRob)
			continue;
		neighbors=getAdjacentHexes(myCities.at(i).getLocation());
		for(unsigned int j=0; j<neighbors.size(); j++)
			tempHexes.push_back(neighbors[j]);
	}
	possibleBlockedHexes=removeDuplicates(tempHexes);
	maxIndex=-1;
	maxValue=-100;
	//cout<<"Player "<<robbingPlayer<<" is robbing player "<<playerToRob<<endl;
	//cout<<"Number of possible blocked hexes "<<possibleBlockedHexes.size()<<endl;
	for(unsigned int i=0; i<possibleBlockedHexes.size(); i++){
		value=0;
		checkPoints=getAdjacentIntersections(possibleBlockedHexes.at(i).getLocation());
		for(unsigned int j=0; j<checkPoints.size(); j++){
			index1=findSettlementIndex(checkPoints.at(j));
			index2=findCityIndex(checkPoints.at(j));
			if(index1!=-1){
				owner=mySettlements.at(index1).getPlayerNum();
				//Point p1=mySettlements.at(index1).getLocation();
				//cout<<"Settlement at "<<p1.getX()<<" "<<p1.getY()<<" owned by player "<<owner<<endl;
				if(owner!=robbingPlayer)
					value+=1;
				else
					value-=1;
			}
			if(index2!=-1){
				owner=myCities.at(index2).getPlayerNum();
				//Point p2=myCities.at(index2).getLocation();
				//cout<<"City at "<<p2.getX()<<" "<<p2.getY()<<" owned by player "<<owner<<endl;
				if(owner!=robbingPlayer)
					value+=2;
				else
					value-=2;
			}
		}
		value*=possibleBlockedHexes.at(i).getValue();
		if(value>maxValue){
			maxIndex=i;
			maxValue=value;
		}
	}
	Point p3=possibleBlockedHexes.at(maxIndex).getLocation();
	//cout<<"Player "<<robbingPlayer<<" chose to rob player "<<playerToRob<<" at location: "<<p3.getX()<<" "<<p3.getY()<<endl;
	return possibleBlockedHexes.at(maxIndex).getLocation();
}
