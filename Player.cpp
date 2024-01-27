/*******************************************************************************************
* Stores a copy of the player's game board which is passed back to the Game class after any 
* changes are made. Also keeps track of player resource types, player development cards, and 
* the number of points this player has (including hidden development cards). If the player 
* wins the game, the turn ends and the player returns this information to the game class.
*
* Includes abstract methods that the class for each player type must override.
*******************************************************************************************/
#include "Player.h"

template<typename T, int size>
int length(T(&)[size]){return size;}

Player::Player(int n, string c, string t, int p, bool v)
{
	playerNum = n;
	color = c;
	playerType = t;
	numPlayers = p;
	isVisible = v;
	for (int i = 0; i < length(resources); i++)
		resources[i] = 0;
	for (int i = 0; i < length(tempResources); i++)
		tempResources[i] = 0;
	for (int i = 0; i < length(tradeRates); i++)
		tradeRates[i] = 4;
	for (int i = 0; i < length(tempTradeRates); i++)
		tempTradeRates[i] = 4;
	// Keeps track of the number of probability "dots" the player has on each resource
	for (int i = 0; i < length(resourcePoints); i++)
		resourcePoints[i] = 0;
	// Player will need to keep track of its own score because of hidden victory point cards
	score = 0;
	numCities = 0;
	numSettlements = 0;
	numRoads = 0;
}
int Player::getScore() const { return score; }
int Player::getNumCities() const { return numCities; }
int Player::getNumSettlements() const { return numSettlements; }
int Player::getNumRoads() const { return numRoads; }
void Player::getResources(int resourcesToReplace[]) const {
	for (int i = 0; i < 5; i++)
		resourcesToReplace[i] = resources[i];
	}
void Player::setResources(const int newResources[]) {
	for (int i = 0; i < 5; i++)
		resources[i] = newResources[i];
}
void Player::setScore(const int s) { score = s; }
void Player::setNumCities(const int c) { numCities = c; }
void Player::setNumSettlements(const int s) { numSettlements = s; }
void Player::setNumRoads(const int r) { numRoads = r; }
void Player::setVisible(const bool v) { isVisible = v; }
void Player::resetBoard(const Board& b) { currentBoard = b; }

// Clear all of the player's data
void Player::resetData() {
	for (int i = 0; i < length(resources); i++)
		resources[i] = 0;
	for (int i = 0; i < length(tradeRates); i++)
		tradeRates[i] = 4;
	for (int i = 0; i < length(tempTradeRates); i++)
		tempTradeRates[i] = 4;
	// Keeps track of the number of probability "dots" the player has on each resource
	for (int i = 0; i < length(resourcePoints); i++)
		resourcePoints[i] = 0;
	// Player will need to keep track of its own score because of hidden victory point cards
	score = 0;
	numCities = 0;
	numSettlements = 0;
	numRoads = 0;
}

// Add a resource by index for the player, and add 1 to the player's number of resources
void Player::gainResource(int i) {
	if (i < 0 || i > 4)
		cerr << "ERROR: Player " << playerNum << " tried to collect resource number "
			<< i << " which is not a valid resource" << endl;
	else {	
		resources[i]++;
		currentBoard.gainResource(playerNum);
	}
}

// Remove a resource by index for the player, and subtract 1 from the player's number of resources
void Player::loseResource(int i) {
	if (i < 0 || i > 4)
		cerr << "ERROR: Player " << playerNum << " tried to discard resource number "
			<< i << " which is not a valid resource" << endl;
	else if (resources[i] < 1)
		cerr << "ERROR: Player " << playerNum << " tried to discard resource number "
			<< i << " but has none of this resource" << endl;
	else {
		resources[i]--;
		currentBoard.loseResource(playerNum);
	}
}

// TODO: consider using an enum instead of having these functions

// Return the text corresponding to a resource index
string Player::getResourceType(int i) {
	switch (i) {
	case 0: return "ore";
	case 1: return "wheat";
	case 2: return "sheep";
	case 3: return "brick";
	case 4: return "wood";
	default: return "";
	}
}

// Return the index corresponding to a resource text
int Player::getResourceIndex(string t) {
	if (t == "ore") return 0;
	else if (t == "wheat") return 1;
	else if (t == "sheep") return 2;
	else if (t == "brick") return 3;
	else if (t == "wood") return 4;
	else return -1;
}

// Add a resource by text for the player, and add 1 to the player's number of resources
void Player::addResource(string t) {
	if (getResourceIndex(t) != -1)
		gainResource(getResourceIndex(t));
	else
		cerr << "ERROR: Player " << playerNum << " tried to collect resource "
			<< t << " which is not a valid resource" << endl;
}

// Trade a resource for another resource with a port or the bank
void Player::portResource(int o, int n) {
	if (o < 0 || o > 4)
		cerr << "ERROR: Player " << playerNum << " tried to trade away invalid resource number "
			<< o << " using a port" << endl;
	else if (n < 0 || n > 4)
		cerr << "ERROR: Player " << playerNum << " tried to trade for invalid resource number "
			<< n << " using a port" << endl;
	else if (resources[o] < tradeRates[o])
		cerr << "ERROR: Player " << playerNum << " tried to trade away resource number " << o 
			<< " using a port, but does not have enough of this resource to make the trade " << endl;
	else {
		resources[n]++;
		resources[o] -= tradeRates[o];

		if (isVisible)
			cout << "Player " << playerNum << " just traded " << tradeRates[o] << " "
				<< getResourceType(o) << " for 1 " << getResourceType(n) << " using a port" << endl;
	}
}

// Trade a resource for another resource with a port or the bank using temporary resources
void Player::tempPortResource(int o, int n) {
	if (o < 0 || o > 4)
		cerr << "ERROR: Player " << playerNum << " tried to trade away invalid resource number "
		<< o << " using a port" << endl;
	else if (n < 0 || n > 4)
		cerr << "ERROR: Player " << playerNum << " tried to trade for invalid resource number "
		<< n << " using a port" << endl;
	else if (resources[o] < tradeRates[o])
		cerr << "ERROR: Player " << playerNum << " tried to trade away resource number " << o
		<< " using a port, but does not have enough of this resource to make the trade " << endl;
	else {
		tempResources[n]++;
		tempResources[o] -= tradeRates[o];
	}
}

// Count the total number of resources owned by a player
int Player::getTotalResources(){
	int totalResources = 0;
	for (int i = 0; i < length(resources); i++)
		totalResources += resources[i];
	return totalResources;
}

// Update the player's resource access based on a new settlement or city
void Player::updateResourcePoints(Point p) {
//	cout << "Updating resource points for player " << playerNum << endl;
	vector<Hexagon> adjacentHexes = currentBoard.getAdjacentHexes(p);
	for (int i = 0; i < adjacentHexes.size(); i++) {
		int resourceIndex = getResourceIndex(adjacentHexes[i].getType());
		resourcePoints[resourceIndex] += adjacentHexes[i].getValue();
	}
}

// Adjust the trade rates of the player based on a newly acquired port
void Player::gainPortPower(string t){
	if (t == "general") {
		for (int i = 0; i < length(tradeRates); i++)
			if (tradeRates[i] == 4)
				tradeRates[i] = 3;
	}
	else if (t == "ore")
		tradeRates[0] = 2;
	else if (t == "wheat")
		tradeRates[1] = 2;
	else if (t == "sheep")
		tradeRates[2] = 2;
	else if (t == "brick")
		tradeRates[3] = 2;
	else if (t == "wood")
		tradeRates[4] = 2;
	else
		cerr << "ERROR: Invalid port type " << t << " gained by player " << playerNum << endl;
}

// Temporarily adjust the trade rates of the player based on a port location being considered
void Player::tempGainPortPower(string t) {
	if (t == "general") {
		for (int i = 0; i < length(tempTradeRates); i++)
			if (tempTradeRates[i] == 4)
				tempTradeRates[i] = 3;
	}
	else if (t == "ore")
		tempTradeRates[0] = 2;
	else if (t == "wheat")
		tempTradeRates[1] = 2;
	else if (t == "sheep")
		tempTradeRates[2] = 2;
	else if (t == "brick")
		tempTradeRates[3] = 2;
	else if (t == "wood")
		tempTradeRates[4] = 2;
	else
		cerr << "ERROR: Invalid port type " << t << " gained by player " << playerNum << " while testing" << endl;
}

// Choose a random resource to be stolen or discarded
int Player::getRandomResource(){
	int total = getTotalResources();
	if (total == 0)
		return -1;
	srand ( (unsigned int)time(NULL) );
	int resourceNum=rand()%total;
	if (0 <= resourceNum && resourceNum < resources[0])
		return 0;
	else if (resources[0] <= resourceNum && resourceNum < resources[0] + resources[1])
		return 1;
	else if (resources[0] + resources[1] <= resourceNum && resourceNum < resources[0] + resources[1] + resources[2])
		return 2;
	else if (total - resources[4] <= resourceNum && resourceNum < total)
		return 4;
	else
		return 3;
}

// Print the player's current resources to the screen
void Player::printResources() const {
	cout << "Player " << playerNum << " has " << score << " points, and " << resources[0]
		<< " ore, " << resources[1] << " wheat, " << resources[2] << " sheep, "
		<< resources[3] << " brick, and " << resources[4] << " wood" << endl;
}
