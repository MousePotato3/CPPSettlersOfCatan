#include "Player.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
using namespace std;

template<typename T, int size>
int length(T(&)[size]){return size;}

Player::Player()
{
	points=0;
	for(int i=0; i<length(resources); i++)
		resources[i]=0;
	for(int i=0; i<length(tradeRates); i++)
		tradeRates[i]=4;
}
int Player::getPoints(){return points;}
int Player::getNumSettlements(){return settlements.size();}
int Player::getNumCities(){return cities.size();}
int Player::getNumRoads(){return roads.size();}
vector<Settlement> Player::getSettlements(){return settlements;}
vector<City> Player::getCities(){return cities;}
vector<Road> Player::getRoads(){return roads;}
vector<Point> Player::getCityPoints(){return newCityPoints;}
vector<Point> Player::getSettlePoints(){return newSettlePoints;}
vector<DoublePoint> Player::getRoadPoints(){return newRoadPoints;}
void Player::getResources(int resourcesToReplace[]){
	for(int i=0;i<5;i++)
		resourcesToReplace[i]=resources[i];
	}
void Player::setResources(int newResources[]){
	for(int i=0;i<5;i++)
		resources[i]=newResources[i];
}
void Player::setPoints(int newPoints){points=newPoints;}
void Player::setVisible(bool b){isVisible=b;}
void Player::resetBoard(Board b){board=b;}
void Player::resetData(){
	settlements.clear();
	cities.clear();
	roads.clear();
}
void Player::gainResource(int i){resources[i]++;}
void Player::loseResource(int i){resources[i]--;}
void Player::addSettlement(Point p, int n){
	settlements.push_back(Settlement(p,n));
}
void Player::addCity(Point p, int n, bool removeSettlement){
	cities.push_back(City(p,n));
	if(removeSettlement){
		int indexToRemove=findSettlementIndex(p);
		settlements.at(indexToRemove)=settlements.back();
		settlements.pop_back();
	}
}
void Player::addRoad(Point p1, Point p2, int n){
	if(p1.getX()>p2.getX())
		roads.push_back(Road(p2,p1,n));
	else
		roads.push_back(Road(p1,p2,n));
}
string Player::getResourceType(int i){
	switch(i){
		case 0: return "ore";
		case 1: return "wheat";
		case 2: return "sheep";
		case 3: return "clay";
		case 4: return "wood";
		default: return "";
	}
}
void Player::addResource(string t){
	if(t=="ore")
		resources[0]++;
	else if(t=="wheat")
		resources[1]++;
	else if(t=="sheep")
		resources[2]++;
	else if(t=="clay")
		resources[3]++;
	else if(t=="wood")
		resources[4]++;
	else if(t=="desert")
		return;
	else
		cout<<"ERROR: Undefined type in addResource"<<endl;
}
int Player::findSettlementIndex(Point p){
	for(unsigned int i=0; i<settlements.size(); i++)
		if(p.equals(settlements.at(i).getLocation()))
			return i;
	return -1;
}
int Player::getTotalResources(){
	int totalResources=0;
	for(int i=0; i<length(resources); i++)
		totalResources+=resources[i];
	return totalResources;
}
int Player::getTotalSettlements(){return settlements.size();}
void Player::gainPortPower(string t){
	if(t=="general"){
		for(int i=0; i<length(tradeRates); i++)
			if(tradeRates[i]==4)
				tradeRates[i]=3;
	}
	else if(t=="ore")
		tradeRates[0]=2;
	else if(t=="wheat")
		tradeRates[1]=2;
	else if(t=="sheep")
		tradeRates[2]=2;
	else if(t=="clay")
		tradeRates[3]=2;
	else if(t=="wood")
		tradeRates[4]=2;
}

int Player::getResourceToSteal(){
	int total=getTotalResources();
	if(total==0)
		return -1;
	srand ( (unsigned int)time(NULL) );
	int resourceNum=rand()%total;
	if(0<=resourceNum && resourceNum<resources[0])
		return 0;
	else if(resources[0]<=resourceNum && resourceNum<resources[0]+resources[1])
		return 1;
	else if(resources[0]+resources[1]<=resourceNum && resourceNum<resources[0]+resources[1]+resources[2])
		return 2;
	else if(total-resources[4]<=resourceNum && resourceNum<total)
		return 4;
	else
		return 3;
}

void Player::printResources(int turn){
	cout<<"Player "<<playerNum<<" resources on turn "<<turn<<": ";
	for(int i=0;i<length(resources);i++){
		assert(resources[i]>=0);
		cout<<resources[i]<<" ";
	}
	cout<<endl;
}