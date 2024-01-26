/*******************************************************************************************
* Keeps track of the location and number for each resource on the Settlers of Catan board,
* along with the settlements, cities, roads, and number of resources and development cards
* for each player, as well as the owner (if any) of longest road and largest army.
* Also keeps track of the winner of the game and returns this information to the Game class.
* Does not keep track of player resource types, player development cards, or the number
* of points each player has (since this could be affected by hidden development cards).
*******************************************************************************************/
#include "Board.h"

const int Board::hexNumbers[18] = { 5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11 };

/* Helper functions defined outside the Board class */
template<typename T, int size>
int length(T(&)[size]) { return size; }

template<typename T>
vector<T> removeDuplicates(vector<T> s) {
	bool duplicates = false;
	vector<T> v;
	for (unsigned int i = 0; i < s.size(); i++) {
		duplicates = false;
		for (unsigned int j = (i + 1); j < s.size(); j++)
			if (s.at(i) == s.at(j))
				duplicates = true;
		if (duplicates == false)
			v.push_back(s.at(i));
	}
	return v;
}

// Structure to keep track of the Settlers of Catan board, all of the objects 
// on the board, and each player's score, resources, and development cards
Board::Board()
{
	// Identify the coordinates of the center of the screen
	centerX = int(GetSystemMetrics(SM_CXSCREEN) / 2.0);
	centerY = int(GetSystemMetrics(SM_CYSCREEN) / 2.0);

	// Assign the width and height of each hexagon that makes up the Catan board
	height = int(centerY / 5.6);
	width = int(height * 1.15);
	smallWidth = int(width / 2.0);

//	cout << "CenterX: " << centerX << "CenterY: " << centerY << endl;
//	cout << "Width: " << width << "Height: " << height << "SmallWidth " << smallWidth << endl;

	// Initialize all of the data that the Board class will use
	hexIntersections = {};
	tiles = {};
	ports = {};
	settlements = {};
	cities = {};
	roads = {};
	numResources = {};
	playerScores = {};
	numPlayers = 0; // Initialized in setNumPlayers
	turnNumber = 0;
	winner = -1;
	isVisible = true;
}
vector<Settlement> Board::getSettlements() const { return settlements; }
vector<City> Board::getCities() const { return cities; }
vector<Road> Board::getRoads() const { return roads; }
vector<int> Board::getNumResources() const { return numResources; }
vector<int> Board::getPlayerScores() const { return playerScores; }
Point Board::getRobberLocation() const { return robberLocation; }
int Board::getTurnNumber() const { return turnNumber; }
int Board::getWinner() const { return winner; }
int Board::getNumResources(int n) const { 
	if (0 < n and n <= numPlayers)
		return numResources.at(n - 1);
	else {
		cerr << "Attempted to get the number of resources for invalid player " << n << endl;
		return -1;
	}
}
int Board::getPlayerScore(int n) const { 
	if (0 < n and n <= numPlayers)
		return playerScores.at(n - 1);
	else {
		cerr << "Attempted to get the score for invalid player " << n << endl;
		return -1;
	}
}
Hexagon Board::getHexagon(int i) const { return tiles.at(i); }
Port Board::getPort(int i) const { return ports.at(i); }
vector<Point> Board::getHexIntersections() const { return hexIntersections; }
void Board::setVisible(const bool b) { isVisible = b; }
void Board::setNumPlayers(const int p) { 
	numPlayers = p; 
	for (int i = 0; i < numPlayers; i++) {
		numResources.push_back(0);
		playerScores.push_back(0);
	}
}
void Board::setRobberLocation(const Point& p) {
	robberLocation.setX(p.getX());
	robberLocation.setY(p.getY());
}
void Board::setTurnNumber(const int t) {
	turnNumber = t;
}
void Board::setWinner(const int w) {
	winner = w;
}
void Board::setNumResources(const int n, const int r) {
	if (0 < n and n <= numPlayers)
		numResources.at(n - 1) = r;
	else
		cerr << "Attempted to set the number of resources for invalid player " << n << endl;
}
void Board::setPlayerScore(const int n, const int s) {
	if (0 < n and n <= numPlayers)
		playerScores.at(n - 1) = s;
	else
		cerr << "Attempted to set the score for invalid player " << n << endl;
}
void Board::resetDice() { d.resetDice(); }
int Board::rollDice() { return d.rollDice(); }
void Board::gainResource(int i) { numResources[i]++; }
void Board::loseResource(int i) {
	if (numResources.at(i) > 0)
		numResources.at(i)--;
	else
		cerr << "Player " << i + 1 << " tried to give up a resource but had none left" << endl;
}

// Make sure that the player number is valid and the settlement can legally be placed at the location.
// Player class needs to check that the player has a settlement left to place before calling this method.
void Board::addSettlement(Point p, int n, bool initialPlacement) {
	if (n > numPlayers || n < 1)
		cerr << "ERROR: Attempted to place a settlement for invalid player number " << n << endl;
	else if (not (legalPlacement(p)))
		cerr << "ERROR: Player " << n << "tried to place a settlement at location (" << p.getX()
			<< ", " << p.getY() << ") which is too close to existing settlements" << endl;
//	else if (!initialPlacement && numResources.at(n - 1) < 4) {
//		cerr << "ERROR: Player " << n << " tried to place a settlement but does not have enough resources to do so" << endl;
//	}
	else {
		if (!initialPlacement) {
//			numResources.at(n - 1) -= 4;
			cout << "Player " << n << " scored a point on turn " << turnNumber
				<< " by placing a settlement at (" << p.getX() << ", " << p.getY() << ")" << endl;
		}
		settlements.push_back(Settlement(p, n));
		playerScores[n]++;
	}
	cout << "Done placing a settlement" << endl;
}

// Make sure that the player number is valid and the player has a settlement at the specified location.
// Player class needs to check that the player has a city left to place before calling this method.
void Board::addCity(Point p, int n, bool initialPlacement) {
	if (n > numPlayers || n < 1)
		cerr << "ERROR: Attempted to place a city for invalid player number " << n << endl;
	int settlementIndex = findSettlementIndex(p, n);
		
	if (settlementIndex == -1)
		cerr << "ERROR: Player " << n << " tried to place a city at location (" << p.getX()
			<< ", " << p.getY() << ") but does not have a settlement at this location" << endl;

	// Remove the player's settlement and replace it with a city
	else {
		if (!initialPlacement) {
//			numResources.at(n - 1) -= 5;
			cout << "Player " << n << " scored a point on turn " << turnNumber
				<< " by placing a city at (" << p.getX() << ", " << p.getY() << ")" << endl;
		}

		settlements.erase(settlements.begin() + settlementIndex);
		cities.push_back(City(p, n));
		playerScores[n]++;
	}
}

// Make sure that the player number is valid, the player is placing a road on an unoccupied location 
// and the player already has a road connected to the location (or a settlement for initial placements).
// Player class needs to check that the player has a road left to place before calling this method.
void Board::addRoad(Point p1, Point p2, int n, bool initialPlacement) {
	if (n > numPlayers || n < 1)
		cerr << "ERROR: Attempted to place a road for invalid player number " << n << endl;
	int roadIndex = findRoadIndex(p1, p2);
	cout << "Started placing a road" << endl;
	if (roadIndex != -1) {
		if (p1.getX() < p2.getX() or (p1.getX() == p2.getX() and p1.getY() <= p2.getY()))
			cerr << "ERROR: Player " << n << " tried to place a road between (" << p1.getX()
				<< ", " << p1.getY() << ") and (" << p2.getX() << ", " << p2.getY()
				<< ") but there is already a road connecting these points" << endl;
		else
			cerr << "ERROR: Player " << n << " tried to place a road between (" << int(p2.getX())
				<< ", " << p2.getY() << ") and (" << p1.getX() << ", " << p1.getY()
				<< ") but there is already a road connecting these points" << endl;
	}
	else {
		if (p1.getX() < p2.getX() or (p1.getX() == p2.getX() and p1.getY() <= p2.getY())) {
			if (!initialPlacement) {
//				numResources.at(n - 1) -= 2;
				cout << "Player " << n << " placed a road between (" << p1.getX()
					<< ", " << p1.getY() << ") and (" << p2.getX() << ", " << p2.getY()
					<< ") on turn " << turnNumber << endl;
			}
			roads.push_back(Road(p1, p2, n));
		}
		else {
			if (!initialPlacement) {
//				numResources.at(n - 1) -= 2;
				cout << "Player " << n << " placed a road between (" << p2.getX()
					<< ", " << p2.getY() << ") and (" << p1.getX() << ", " << p1.getY()
					<< ") on turn " << turnNumber << endl;
			}
			roads.push_back(Road(p2, p1, n));
		}
	}
}

// Set up all of the resource tiles on the Catan game board
void Board::initTiles() {
	int i;
	int counter = 0;

	// Create Hexagon objects for each Hexagon on the Catan board and put them in a random order
	tiles.push_back(Hexagon("desert"));
	for (i = 1; i < 4; i++)
		tiles.push_back(Hexagon("ore"));
	for (i = 4; i < 7; i++)
		tiles.push_back(Hexagon("brick"));
	for (i = 7; i < 11; i++)
		tiles.push_back(Hexagon("sheep"));
	for (i = 11; i < 15; i++)
		tiles.push_back(Hexagon("wheat"));
	for (i = 15; i < 19; i++)
		tiles.push_back(Hexagon("wood"));
	random_shuffle(tiles.begin(), tiles.end());

	// Assign locations of the center of each Hexagon on the Catan board, 
	// starting at the top and moving counter-clockwise toward the center 
	hexCenters[0] = Point(centerX, centerY - 2 * height * 2);
	hexCenters[1] = Point(centerX - (width + smallWidth), centerY - 3 * height);
	hexCenters[2] = Point(centerX - (width * 2 + smallWidth * 2), centerY - height * 2);
	hexCenters[3] = Point(centerX - (width * 2 + smallWidth * 2), centerY);
	hexCenters[4] = Point(centerX - (width * 2 + smallWidth * 2), centerY + height * 2);
	hexCenters[5] = Point(centerX - (width + smallWidth), centerY + 3 * height);
	hexCenters[6] = Point(centerX, centerY + 2 * height * 2);
	hexCenters[7] = Point(centerX + (width + smallWidth), centerY + 3 * height);
	hexCenters[8] = Point(centerX + (width * 2 + smallWidth * 2), centerY + height * 2);
	hexCenters[9] = Point(centerX + (width * 2 + smallWidth * 2), centerY);
	hexCenters[10] = Point(centerX + (width * 2 + smallWidth * 2), centerY - height * 2);
	hexCenters[11] = Point(centerX + (width + smallWidth), centerY - 3 * height);
	hexCenters[12] = Point(centerX, centerY - height * 2);
	hexCenters[13] = Point(centerX - (width + smallWidth), centerY - height);
	hexCenters[14] = Point(centerX - (width + smallWidth), centerY + height);
	hexCenters[15] = Point(centerX, centerY + height * 2);
	hexCenters[16] = Point(centerX + (width + smallWidth), centerY + height);
	hexCenters[17] = Point(centerX + (width + smallWidth), centerY - height);
	hexCenters[18] = Point(centerX, centerY);

	// Assign numbers to each Hexagon on the Catan board, except for the desert
	// which initially contains the robber instead of a number
	for (i = 0; i < 19; i++) {
		tiles.at(i).setLocation(hexCenters[i]);
		if (tiles.at(i).getType() != "desert") {
			tiles.at(i).setNumber(hexNumbers[counter]);
			counter++;
		}
		else {
			tiles.at(i).setNumber(1);
			robberLocation = tiles.at(i).getLocation();
		}
	}

	// Assign the location of each intersection at the edge of a hexagon and remove duplicates
	for (i = 0; i < length(hexCenters); i++) {
		int x = hexCenters[i].getX();
		int y = hexCenters[i].getY();

		tempIntersections.push_back(Point(x + width, y));
		tempIntersections.push_back(Point(x - width, y));
		tempIntersections.push_back(Point(x + smallWidth, y + height));
		tempIntersections.push_back(Point(x - smallWidth, y + height));
		tempIntersections.push_back(Point(x + smallWidth, y - height));
		tempIntersections.push_back(Point(x - smallWidth, y - height));
	}
	hexIntersections = removeDuplicates(tempIntersections);
}

// Set up all of the ports on the Catan game board
void Board::initPorts() {
	// Create Port objects for each Port on the Catan board and put them in a random order
	for (int i = 0; i < 4; i++)
		ports.push_back(Port("general"));
	ports.push_back(Port("brick"));
	ports.push_back(Port("ore"));
	ports.push_back(Port("sheep"));
	ports.push_back(Port("wheat"));
	ports.push_back(Port("wood"));
	random_shuffle(ports.begin(), ports.end());

	// Assign locations of the each Port on the Catan board
	portTextLocations[0] = Point(centerX - smallWidth + 15, centerY + 5 * height + 30);
	portPoint1Locations[0] = Point(centerX - smallWidth, centerY + 5 * height);
	portPoint2Locations[0] = Point(centerX + smallWidth, centerY + 5 * height);

	portTextLocations[1] = Point(centerX - (width * 2 + smallWidth * 2) + 10, centerY + 7 * height / 2 + 15);
	portPoint1Locations[1] = Point(centerX - (width + smallWidth * 2), centerY + 2 * height * 2);
	portPoint2Locations[1] = Point(centerX - (width * 2 + smallWidth), centerY + 3 * height);

	portTextLocations[2] = Point(centerX - 3 * (width + smallWidth) + 10, centerY + height / 2 + 15);
	portPoint1Locations[2] = Point(centerX - (3 * width + smallWidth * 2), centerY);
	portPoint2Locations[2] = Point(centerX - (width * 2 + 3 * smallWidth), centerY + height);

	portTextLocations[3] = Point(centerX - 3 * (width + smallWidth) + 10, centerY - 5 * height / 2 + 5);
	portPoint1Locations[3] = Point(centerX - (3 * width + smallWidth * 2), centerY - height * 2);
	portPoint2Locations[3] = Point(centerX - (width * 2 + 3 * smallWidth), centerY - 3 * height);

	portTextLocations[4] = Point(centerX - (width + smallWidth * 2) + 15, centerY - 2 * height * 2 - 5);
	portPoint1Locations[4] = Point(centerX - (width + smallWidth * 2), centerY - 2 * height * 2);
	portPoint2Locations[4] = Point(centerX - width, centerY - 2 * height * 2);

	portTextLocations[5] = Point(centerX + width + 15, centerY - 2 * height * 2 - 5);
	portPoint1Locations[5] = Point(centerX + width + smallWidth * 2, centerY - 2 * height * 2);
	portPoint2Locations[5] = Point(centerX + width, centerY - 2 * height * 2);

	portTextLocations[6] = Point(centerX + (width * 2 + 3 * smallWidth) + 23, centerY - 5 * height / 2 + 5);
	portPoint1Locations[6] = Point(centerX + (3 * width + smallWidth * 2), centerY - height * 2);
	portPoint2Locations[6] = Point(centerX + (width * 2 + 3 * smallWidth), centerY - 3 * height);

	portTextLocations[7] = Point(centerX + (width * 2 + 3 * smallWidth) + 23, centerY + height / 2 + 15);
	portPoint1Locations[7] = Point(centerX + (3 * width + smallWidth * 2), centerY);
	portPoint2Locations[7] = Point(centerX + (width * 2 + 3 * smallWidth), centerY + height);

	portTextLocations[8] = Point(centerX + (width + smallWidth * 2) + 23, centerY + 7 * height / 2 + 15);
	portPoint1Locations[8] = Point(centerX + (width + smallWidth * 2), centerY + 2 * height * 2);
	portPoint2Locations[8] = Point(centerX + (width * 2 + smallWidth), centerY + 3 * height);

	for (unsigned int i = 0; i < ports.size(); i++) {
		ports.at(i).setLocation(portTextLocations[i]);
		ports.at(i).setPortLocations(DoublePoint(portPoint1Locations[i], portPoint2Locations[i]));
		DoublePoint dp = ports.at(i).getPortLocations();
		DoublePoint kp = DoublePoint(portPoint1Locations[i], portPoint2Locations[i]);
	}
}

// Return the tile at the specified location, or -1 if not found
int Board::findHexIndex(Point p){
	for (unsigned int i = 0; i < tiles.size(); i++)
		if (tiles.at(i).getLocation().getX() == p.getX()
			&& tiles.at(i).getLocation().getY() == p.getY())
			return i;
	return -1;
}

// Return the index of the settlement at the specified location, or -1 if not found. 
// If playerNum is not -1, only return a settlement index for the specified player.
int Board::findSettlementIndex(Point p, int n){
	for (unsigned int i = 0; i < settlements.size(); i++)
		if (p == settlements.at(i).getLocation()
			and (n == -1 or n == settlements.at(i).getPlayerNum()))
			return i;
	return -1;
}

// Return the index of the city at the specified location, or -1 if not found. 
// If playerNum is not -1, only return a city index for the specified player.
int Board::findCityIndex(Point p, int n){
	for (unsigned int i = 0; i < cities.size(); i++)
		if (p == cities.at(i).getLocation()
			and (n == -1 or n == cities.at(i).getPlayerNum()))
			return i;
	return -1;
}

// Return the index of the road at the specified location, or -1 if not found. 
// If playerNum is not -1, only return a road index for the specified player.
int Board::findRoadIndex(Point p1, Point p2, int n){
	for (unsigned int i = 0; i < roads.size(); i++) {
		if ((p1 == roads.at(i).getLocation1() && p2 == roads.at(i).getLocation2())
			|| (p2 == roads.at(i).getLocation1() && p1 == roads.at(i).getLocation2())
			and (n == -1 or n == roads.at(i).getPlayerNum()))
			return i;
	}
	return -1;
}

// Return True if the two points are adjacent to each other or False if they are not
bool Board::isAdjacent(Point p1, Point p2){
	double distance = sqrt(pow((float)(p1.getX() - p2.getX()), 2) + pow((float)(p1.getY() - p2.getY()), 2));
	if (abs((int)distance - smallWidth * 2) < 2)
		return true;
	return false;
}

// Return the type of port at a specified location, or a blank string if the location is not at a port
string Board::getPortType(Point p) {
	for (unsigned int i = 0; i < ports.size(); i++) {
		if (p == ports.at(i).getPortLocations().getP1()
			|| p == ports.at(i).getPortLocations().getP2())
			return ports.at(i).getType();
	}
	return "";
}

// Create a list of every point adjacent to an intersection (should not include the point itself),
// for the purpose of determining where a settlement or a road can be built
vector<Point> Board::getAdjacentIntersections(Point p){
	vector<Point> myPoints;

	for (unsigned int i = 0; i < hexIntersections.size(); i++)
		if (isAdjacent(p, hexIntersections.at(i)))
			myPoints.push_back(hexIntersections.at(i));
	return myPoints;
}

// Create a list of every hexagon adjacent to an intersection, 
// for the purpose of determining which tiles to collect resources from
vector<Hexagon> Board::getAdjacentHexes(Point p){
	int x = p.getX();
	int y = p.getY();
	vector<Hexagon> adjacentHexes;
	int adjacentIndices[6] = { findHexIndex(Point(x + width, y)), findHexIndex(Point(x - width, y)),
		findHexIndex(Point(x + smallWidth, y + height)), findHexIndex(Point(x - smallWidth, y + height)),
		findHexIndex(Point(x + smallWidth, y - height)), findHexIndex(Point(x - smallWidth, y - height)) };
		
	for (int i = 0; i < length(adjacentIndices); i++)
		if (adjacentIndices[i] != -1)
			adjacentHexes.push_back(tiles.at(adjacentIndices[i]));
	return adjacentHexes;
}

// Determine whether an intersection is connected to a player's road network
bool Board::isConnected(Point p, int n) {
	for (unsigned int i = 0; i < roads.size(); i++)
		if (roads.at(i).getLocation1() == p || roads.at(i).getLocation2() == p)
			return true;
	return false;
}

// Determine whether an intersection is legal for placing a settlement
// (no settlements or cities at the intersection or any adjacent intersections)
bool Board::legalPlacement(Point p){
	bool isHexLegal = true;
	if (findSettlementIndex(p) != -1 || findCityIndex(p) != -1)
		isHexLegal = false;
	vector<Point> checkPoints = getAdjacentIntersections(p);
	for (unsigned int i = 0; i < checkPoints.size(); i++)
		if (findSettlementIndex(checkPoints.at(i)) != -1 || findCityIndex(checkPoints.at(i)) != -1)
			isHexLegal = false;
	return isHexLegal;
}

// Return a list of possible locations for a player to build a city (replaces a settlement)
vector<Point> Board::getPossibleCityLocations(int n) {
	vector<Point> possibleCityLocations;
	for (unsigned int i = 0; i < settlements.size(); i++)
		if (settlements.at(i).getPlayerNum() == n)
			possibleCityLocations.push_back(settlements.at(i).getLocation());
	return possibleCityLocations;
}

// Return a list of possible locations for a player to build a settlement
vector<Point> Board::getPossibleSettlementLocations(int n){
	vector<Point> playerRoadPoints;
	vector<Point> possibleSettleLocations;

	// Create a list of all locations currently connected to the player's roads 
	for(unsigned int i=0; i<roads.size(); i++){
		if (roads.at(i).getPlayerNum() == n) {
			playerRoadPoints.push_back(roads.at(i).getLocation1());
			playerRoadPoints.push_back(roads.at(i).getLocation2());
		}
	}
	removeDuplicates(playerRoadPoints);		
	
	// Include only locations where a settlement can be legally placed
	for(unsigned int i=0; i<playerRoadPoints.size(); i++)
		if(legalPlacement(playerRoadPoints.at(i)))
			possibleSettleLocations.push_back(playerRoadPoints.at(i));
	return possibleSettleLocations;
}

// Return a list of possible locations for a player to build a road
vector<DoublePoint> Board::getPossibleRoadLocations(int n) {
	vector<Point> tempRoadPoints;
	vector<Point> playerRoadPoints;
	vector<Point> neighbors;
	vector<DoublePoint> roadLocations;
	int mySettlementIndex, myCityIndex, anySettlementIndex, anyCityIndex;

	// Create a list of all locations currently connected to the player's roads
	for (unsigned int i = 0; i < roads.size(); i++) {
		if (roads.at(i).getPlayerNum() == n) {
			tempRoadPoints.push_back(roads.at(i).getLocation1());
			tempRoadPoints.push_back(roads.at(i).getLocation2());
		}
	}
	removeDuplicates(tempRoadPoints);

	// Only keep points not occupied by an opposing player's settlement or city
	for (unsigned int i = 0; i < tempRoadPoints.size(); i++) {
		mySettlementIndex = findSettlementIndex(tempRoadPoints.at(i), n);
		myCityIndex = findCityIndex(tempRoadPoints.at(i), n);
		anySettlementIndex = findSettlementIndex(tempRoadPoints.at(i));
		anyCityIndex = findCityIndex(tempRoadPoints.at(i));

		if ((mySettlementIndex != -1 || anySettlementIndex == -1) 
			and (myCityIndex != -1 || anyCityIndex == -1))
			playerRoadPoints.push_back(tempRoadPoints.at(i));
	}

	// Create a list of possible roads, removing any for which the road already exists
	for (unsigned int i = 0; i < playerRoadPoints.size(); i++) {
		neighbors = getAdjacentIntersections(playerRoadPoints.at(i));
		for (unsigned int j = 0; j < neighbors.size(); j++)
			if (findRoadIndex(playerRoadPoints.at(i), neighbors.at(j), -1) == -1)
				roadLocations.push_back(DoublePoint(playerRoadPoints.at(i), neighbors.at(j)));
	}

	// Remove duplicates from the list of possible roads and return the list
	removeDuplicates(roadLocations);
	return roadLocations;
}
