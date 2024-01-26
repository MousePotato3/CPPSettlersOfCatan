/*******************************************************************************************
* An instance of the abstract Player class that makes many of its decisions
* by running Monte Carlo simulations to determine which move is best
*******************************************************************************************/
#include "MonteComp.h"

template<typename T, int size>
int length(T(&)[size]){return size;}

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

MonteComp::MonteComp(int n, string c, string t, int p, bool v) :Player(n, c, t, p, v) {
	srand((unsigned int)time(NULL));
}

// Calculate the expected value of a possible settlement or city location, based on
// total and variety of resources and available ports (including newly acquired ports)
int MonteComp::getHexValue(Point p) {
	vector<Hexagon> adjacentHexes = currentBoard.getAdjacentHexes(p);
	int hexValue = 0;
	for (unsigned int i = 0; i < adjacentHexes.size(); i++) {
		hexValue += adjacentHexes.at(i).getValue();
	}
	return hexValue;
}

// Calculate the expected value of a possible road location, 
// based on settlements that could potentially be acquired nearby
int MonteComp::getRoadValue(DoublePoint d) {
	int roadValue = 0;
	vector<Point> adjacentIntersections;

	// Only count the value of locations that are not already part of a player's road network
	if (!currentBoard.isConnected(d.getP1(), playerNum)) {
		roadValue += getHexValue(d.getP1());
		adjacentIntersections = currentBoard.getAdjacentIntersections(d.getP1());
		for (unsigned int i = 0; i < adjacentIntersections.size(); i++) {
			if (currentBoard.legalPlacement(adjacentIntersections.at(i)))
				roadValue += getHexValue(adjacentIntersections.at(i)) / 10;
		}
	}

	if (!currentBoard.isConnected(d.getP2(), playerNum)) {
		roadValue += getHexValue(d.getP2());
		adjacentIntersections = currentBoard.getAdjacentIntersections(d.getP2());
		for (unsigned int i = 0; i < adjacentIntersections.size(); i++) {
			if (currentBoard.legalPlacement(adjacentIntersections.at(i)))
				roadValue += getHexValue(adjacentIntersections.at(i)) / 10;
		}
	}

	return roadValue;
}

// Choose the best possible initial settlement location based on resource
// probability, resource diversity, access to ports, and a random factor
Point MonteComp::chooseInitialSettlementLocation(Board b) {
	currentBoard = b;
	double hexValue;
	double randomNumber;
	double maxValue = -10.0;
	int maxIndex = -1;

	hexIntersections = currentBoard.getHexIntersections();
	for (unsigned int i = 0; i < hexIntersections.size(); i++) {
		if (currentBoard.legalPlacement(hexIntersections.at(i))) {
			randomNumber = rand() / 0.5;
			hexValue = getHexValue(hexIntersections.at(i)) + randomNumber;
			if (hexValue > maxValue) {
				maxValue = hexValue;
				maxIndex = i;
			}
		}
	}
	numSettlements++;
	score++;
	return hexIntersections.at(maxIndex);
}

// Create a list of every intersection that a road can connect to the initial
// settlement location, and choose an intersection from the list at random
// TODO: Choose the road direction based on Monte Carlo simulation
Point MonteComp::chooseInitialRoadLocation(Point p) {
	vector<Point> possibleRoadPoints = currentBoard.getAdjacentIntersections(p);
	int roadNum = rand() % possibleRoadPoints.size();
	numRoads++;
	return possibleRoadPoints.at(roadNum);
}

// Discard resources from the player at random
void MonteComp::discard() {
	int numberToDiscard = getTotalResources() / 2;
	if (isVisible)
		cout << "Player " << playerNum << " discarded " << numberToDiscard
		<< " resources on turn " << currentBoard.getTurnNumber() << endl;
	for (int i = 0; i < numberToDiscard; i++) {
		int maxIndex = 0;
		int maxResources = resources[0];
		for (int j = 1; j < length(resources); j++) {
			if (resources[j] >= maxResources) {
				maxIndex = j;
				maxResources = resources[j];
			}
		}
		resources[maxIndex]--;
	}
}

// Generate a random number between 0 and 2 and add it to each player's score, 
// to somewhat randomly choose a player who is close to the lead to rob from.
// Only choose a player that has a resource to steal, if possible.
int MonteComp::getPlayerToRob() {
	double maxPoints = 0.0;
	int playerToRob = -1;
	int cannotRob = 1;

	// Determine whether any player has a resource to steal
	vector<int> numResources = currentBoard.getNumResources();
	for (unsigned int i = 0; i < numResources.size(); i++) {
		if (i != (playerNum - 1) && numResources.at(i) > 0)
			cannotRob = 0;
	}

	// Select a player to rob randomly who is leading or close to the lead
	vector<int> playerScores = currentBoard.getPlayerScores();
	for (unsigned int i = 0; i < playerScores.size(); i++) {
		double randomNumber = rand() / 0.5;
		if (((cannotRob == 0 && numResources.at(i) > 0) || cannotRob == 1)
			&& i != playerNum - 1 && playerScores.at(i) + randomNumber > maxPoints) {
			maxPoints = playerScores.at(i) + randomNumber;
			playerToRob = i + 1;
		}
	}

	return playerToRob;
}

// Identify the best location to block to slow down the progress of the player being robbed
Point MonteComp::getPointToBlock(int playerToRob) {
	vector<Hexagon> neighbors;
	vector<Point> checkPoints;
	vector<Hexagon> tempHexes;
	vector<Hexagon> possibleBlockedHexes;
	vector<int> maxIndices;
	vector<int> playerScores = currentBoard.getPlayerScores();
	vector<Settlement> settlements = currentBoard.getSettlements();
	vector<City> cities = currentBoard.getCities();
	int randomIndex, hexValue, maxValue, mySettlementIndex, myCityIndex, anySettlementIndex, anyCityIndex;

	// Create a list of possible locations from which to block playerToRob
	for (unsigned int i = 0; i < settlements.size(); i++) {
		if (settlements.at(i).getPlayerNum() != playerToRob)
			continue;
		neighbors = currentBoard.getAdjacentHexes(settlements.at(i).getLocation());
		for (unsigned int j = 0; j < neighbors.size(); j++)
			tempHexes.push_back(neighbors[j]);
	}
	for (unsigned int i = 0; i < cities.size(); i++) {
		if (cities.at(i).getPlayerNum() != playerToRob)
			continue;
		neighbors = currentBoard.getAdjacentHexes(cities.at(i).getLocation());
		for (unsigned int j = 0; j < neighbors.size(); j++)
			tempHexes.push_back(neighbors[j]);
	}
	possibleBlockedHexes = removeDuplicates(tempHexes);

	maxValue = -100;

	// cout << "Player " << robbingPlayer << " is robbing player " << playerToRob << endl;
	// cout << "Number of possible blocked hexes " << possibleBlockedHexes.size() << endl;

	// Assign a significant negative value if we are blocking ourselves. 
	// Otherwise the value of a location to block is sum(playerPoints)*hexValue, 
	// where playerPoints count double for a city compared to a settlement.
	for (unsigned int i = 0; i < possibleBlockedHexes.size(); i++) {
		hexValue = 0;
		checkPoints = currentBoard.getAdjacentIntersections(possibleBlockedHexes.at(i).getLocation());
		for (unsigned int j = 0; j < checkPoints.size(); j++) {
			mySettlementIndex = currentBoard.findSettlementIndex(checkPoints.at(j), playerNum);
			myCityIndex = currentBoard.findCityIndex(checkPoints.at(j), playerNum);
			anySettlementIndex = currentBoard.findSettlementIndex(checkPoints.at(j));
			anyCityIndex = currentBoard.findCityIndex(checkPoints.at(j));

			// If a settlement is found and the player doesn't own the settlement, increase 
			// the value of blocking the hexagon by the player's score. If the player does 
			// own the settlement, decrease the value by 20 (to discourage a self - block)
			if (anySettlementIndex != -1) {
				if (mySettlementIndex != -1) {
					int blockedPlayer = settlements.at(anySettlementIndex).getPlayerNum() - 1;
					hexValue += playerScores.at(blockedPlayer);
				}
				else {
					hexValue -= 20;
				}
			}
			// If a city is found and the player doesn't own the city, increase the value 
			// of blocking the hexagon by twice the player's score. If the player does 
			// own the city, decrease the value by 40 (to discourage a self - block)
			if (anyCityIndex != -1) {
				if (myCityIndex != -1) {
					int blockedPlayer = cities.at(anySettlementIndex).getPlayerNum() - 1;
					hexValue += playerScores.at(blockedPlayer) * 2;
				}
				else {
					hexValue -= 40;
				}
			}
		}
		hexValue *= possibleBlockedHexes.at(i).getValue();
		if (hexValue > maxValue) {
			maxValue = hexValue;
			maxIndices.clear();
			maxIndices.push_back(i);
		}
		else if (hexValue == maxValue) {
			maxIndices.push_back(i);
		}
	}
	// Choose one of the best possible locations to block randomly
	randomIndex = rand() * int(maxIndices.size());

	return possibleBlockedHexes.at(randomIndex).getLocation();
}

// Return a list of resources to trade to the bank in order to build a city
vector<int> MonteComp::cityPortResources() {
	vector<int> resourcesToTrade;

	// Return an empty list if the player already has the maximum number of cities 
	if (numCities >= 4) {
		resourcesToTrade.clear();
		resourcesToTrade.push_back(-1);
		return resourcesToTrade;
	}

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	// Keep trying to trade resources to the bank via port until a city can be built.
	// Store the resources that are traded to the bank in a list.
	int maxRemainingResources = 0;
	do {
		// If a city can be built, return the list of resources to trade
		if (tempResources[0] >= 3 && tempResources[1] >= 2)
			return resourcesToTrade;

		// Consider trading ore to the bank to build a city
		int maxIndex = 0;
		maxRemainingResources = tempResources[0] - 3 - tradeRates[0];

		// Consider trading wheat to the bank to build a city
		if (tempResources[1] - 2 - tradeRates[1] >= maxRemainingResources) {
			maxIndex = 1;
			maxRemainingResources = tempResources[1] - 2 - tradeRates[1];
		}

		// Consider trading sheep, brick, or wood to the bank to build a city
		for (int i = 2; i < length(tempResources); i++) {
			if (tempResources[i] - tradeRates[i] >= maxRemainingResources) {
				maxIndex = i;
				maxRemainingResources = tempResources[i] - tradeRates[i];
			}
		}

		// Within the player's temporary resources, port the resource chosen to the 
		// bank to get a resource needed to build a city. Add the ported resource to
		// the list of resources that need to be ported to the bank to build a city.
		if (maxRemainingResources >= 0) {
			if (tempResources[0] < 3)
				tempResources[0]++;
			else
				tempResources[1]++;
			resourcesToTrade.push_back(maxIndex);
			tempResources[maxIndex] -= tradeRates[maxIndex];
		}
	} while (maxRemainingResources >= 0);

	// If search fails, set the only resource to trade to -1 as a flag
	resourcesToTrade.clear();
	resourcesToTrade.push_back(-1);
	return resourcesToTrade;
}

// Return a list of resources to trade to the bank in order to build a settlement
vector<int> MonteComp::settlementPortResources() {
	vector<int> resourcesToTrade;

	// Return an empty list if the player already has the maximum number of settlements 
	if (numSettlements >= 5) {
		resourcesToTrade.clear();
		resourcesToTrade.push_back(-1);
		return resourcesToTrade;
	}

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	// Keep trying to trade resources to the bank via port until a settlement can be built.
	// Store the resources that are traded to the bank in a list.
	int maxRemainingResources;
	do {
		// If a settlement can be built, return the list of resources to trade 
		if (tempResources[1] > 0 && tempResources[2] > 0 && tempResources[3] > 0 && tempResources[4] > 0)
			return resourcesToTrade;

		// Consider trading ore to the bank to build a settlement
		int maxIndex = 0;
		maxRemainingResources = tempResources[0] - tradeRates[0];

		// Consider trading wheat, sheep, brick, or wood to the bank to build a settlement
		for (int i = 1; i < length(tempResources); i++) {
			if (tempResources[i] - 1 - tradeRates[i] >= maxRemainingResources) {
				maxIndex = i;
				maxRemainingResources = tempResources[i] - 1 - tradeRates[i];
			}
		}

		// Within the player's temporary resources, port the resource chosen to the 
		// bank to get a resource needed to build a settlement. Add the ported resource to
		// the list of resources that need to be ported to the bank to build a settlement.
		if (maxRemainingResources >= 0) {
			if (tempResources[1] == 0)
				tempResources[1]++;
			else if (tempResources[2] == 0)
				tempResources[2]++;
			else if (tempResources[3] == 0)
				tempResources[3]++;
			else if (tempResources[4] == 0)
				tempResources[4]++;
			resourcesToTrade.push_back(maxIndex);
			tempResources[maxIndex] -= tradeRates[maxIndex];
		}
	} while (maxRemainingResources >= 0);

	// If search fails, set the only resource to trade to -1 as a flag
	resourcesToTrade.clear();
	resourcesToTrade.push_back(-1);
	return resourcesToTrade;
}

// Return a list of resources to trade to the bank in order to build a road
vector<int> MonteComp::roadPortResources() {
	vector<int> resourcesToTrade;

	// Return an empty list if the player already has the maximum number of roads 
	if (numRoads >= 15) {
		resourcesToTrade.clear();
		resourcesToTrade.push_back(-1);
		return resourcesToTrade;
	}

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	int maxRemainingResources;
	do {
		// If a road can be built, return the list of resources to trade 
		if (tempResources[3] > 0 && tempResources[4] > 0)
			return resourcesToTrade;

		// Consider trading ore to the bank to build a road
		int maxIndex = 0;
		maxRemainingResources = tempResources[0] - tradeRates[0];

		// Consider trading wheat or sheep to the bank to build a road
		if (tempResources[1] - tradeRates[1] >= maxRemainingResources) {
			maxIndex = 1;
			maxRemainingResources = tempResources[1] - tradeRates[1];
		}
		if (tempResources[2] - tradeRates[2] >= maxRemainingResources) {
			maxIndex = 2;
			maxRemainingResources = tempResources[2] - tradeRates[2];
		}

		// Consider trading wood or brick to the bank to build a road
		for (int i = 3; i < length(tempResources); i++) {
			if (tempResources[i] - 1 - tradeRates[i] >= maxRemainingResources) {
				maxIndex = i;
				maxRemainingResources = tempResources[i] - 1 - tradeRates[i];
			}
		}

		// Within the player's temporary resources, port the resource chosen to the 
		// bank to get a resource needed to build a road. Add the ported resource to
		// the list of resources that need to be ported to the bank to build a road.
		if (maxRemainingResources >= 0) {
			if (tempResources[3] == 0)
				tempResources[3]++;
			else if (tempResources[4] == 0)
				tempResources[4]++;
			resourcesToTrade.push_back(maxIndex);
			tempResources[maxIndex] -= tradeRates[maxIndex];
		}
	} while (maxRemainingResources >= 0);

	// If search fails, set the only resource to trade to -1 as a flag
	resourcesToTrade.clear();
	resourcesToTrade.push_back(-1);
	return resourcesToTrade;
}

// Build a city for the player, if it is possible to do so 
int MonteComp::placeCity(vector<Point> cityPoints) {
	vector<int> resourcesToTrade = cityPortResources();
	// Return -1 if the player already has the maximum number of cities, 
	// or if there are no locations available for the player to build a city, 
	// or if the player does not have enough resources to build a city 
	if (numCities >= 4 || cityPoints.size() == 0 || (resourcesToTrade.size() > 0 && resourcesToTrade[0] == -1))
		return -1;

	// Run simulations on the default option, which is not to place a city
	int maxValue = runSimulation(currentBoard, resources, score);
	int maxIndex = -1;

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	// First make the necessary port trades to place the city and adjust temporary resources
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (tempResources[0] < 3 && resourcesToTrade.at(i) != 0)
			resourceReceived = 0;
		else if (tempResources[1] < 2 && resourcesToTrade.at(i) != 1)
			resourceReceived = 1;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade.at(i))
				<< " by player " << playerNum << " while trying to build a city" << endl;
		}
		tempPortResource(resourcesToTrade.at(i), resourceReceived);
	}
	tempResources[0] -= 3;
	tempResources[1] -= 2;
	int tempScore = score + 1;

	// Create a test board with a city in every possible location and run Monte Carlo simulations on each board
	for (unsigned int i = 0; i < cityPoints.size(); i++) {
		Board testBoard = currentBoard;
		testBoard.addCity(cityPoints[i], playerNum - 1);		

		int value = runSimulation(testBoard, tempResources, tempScore);
		if (value >= maxValue) {
			maxValue = value;
			maxIndex = i;
		}
	}

	// Return -1 if the simulations showed that it is better not to build a city
	if (maxIndex == -1)
		return -1;

	// Trade resources with the bank until a city can be built
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (resources[0] < 3 && resourcesToTrade.at(i) != 0)
			resourceReceived = 0;
		else if (resources[1] < 2 && resourcesToTrade.at(i) != 1)
			resourceReceived = 1;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade[i])
				<< " by player " << playerNum << " while trying to build a city" << endl;
		}
		portResource(resourcesToTrade.at(i), resourceReceived);
	}

	// Build a city at the best location for the player based on Monte Carlo simulation
	if (resources[0] >= 3 && resources[1] >= 2) {
		resources[0] -= 3;
		resources[1] -= 2;
		currentBoard.addCity(cityPoints.at(maxIndex), playerNum);
		updateResourcePoints(cityPoints.at(maxIndex));
		numCities++;
		score++;
		return maxIndex;
	}

	return -1;
}

// Build a settlement for the player, if it is possible to do so
int MonteComp::placeSettlement(vector<Point> settlementPoints) {
	vector<int> resourcesToTrade = settlementPortResources();
	// Return -1 if the player already has the maximum number of settlements, 
	// or if there are no locations available for the player to build a settlement, 
	// or if the player does not have enough resources to build a settlement
	if (numSettlements >= 5 || settlementPoints.size() == 0 || (resourcesToTrade.size() > 0 && resourcesToTrade[0] == -1))
		return -1;

	// Run simulations on the default option, which is not to place a settlement
	int maxValue = runSimulation(currentBoard, resources, score);
	int maxIndex = -1;

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	// First make the necessary port trades to place the settlement and adjust temporary resources
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (tempResources[1] == 0 && resourcesToTrade.at(i) != 1)
			resourceReceived = 1;
		else if (tempResources[2] == 0 && resourcesToTrade.at(i) != 2)
			resourceReceived = 2;
		else if (tempResources[3] == 0 && resourcesToTrade.at(i) != 3)
			resourceReceived = 3;
		else if (tempResources[4] == 0 && resourcesToTrade.at(i) != 4)
			resourceReceived = 4;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade.at(i))
				<< " by player " << playerNum << " while trying to build a settlement" << endl;
		}
		tempPortResource(resourcesToTrade.at(i), resourceReceived);
	}
	tempResources[1]--;
	tempResources[2]--;
	tempResources[3]--;
	tempResources[4]--;
	int tempScore = score + 1;

	// Create a test board with a settlement in every possible location and run Monte Carlo simulations on each board
	for (unsigned int i = 0; i < settlementPoints.size(); i++) {
		Board testBoard = currentBoard;
		testBoard.addSettlement(settlementPoints[i], playerNum - 1);

		int value = runSimulation(testBoard, tempResources, tempScore);
		if (value >= maxValue) {
			maxValue = value;
			maxIndex = i;
		}
	}

	// Return -1 if the simulations showed that it is better not to build a settlement
	if (maxIndex == -1)
		return -1;

	// Trade resources with the bank until a settlement can be built
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (resources[1] == 0 && resourcesToTrade.at(i) != 1)
			resourceReceived = 1;
		else if (resources[2] == 0 && resourcesToTrade.at(i) != 2)
			resourceReceived = 2;
		else if (resources[3] == 0 && resourcesToTrade.at(i) != 3)
			resourceReceived = 3;
		else if (resources[4] == 0 && resourcesToTrade.at(i) != 4)
			resourceReceived = 4;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade.at(i))
				<< " by player " << playerNum << " while trying to build a settlement" << endl;
		}
		portResource(resourcesToTrade.at(i), resourceReceived);
	}

	// Build a settlement at the best location for the player based on Monte Carlo simulation
	if (resources[1] > 0 && resources[2] > 0 && resources[3] > 0 && resources[4] > 0) {
		resources[1]--;
		resources[2]--;
		resources[3]--;
		resources[4]--;
		currentBoard.addSettlement(settlementPoints.at(maxIndex), playerNum);
		updateResourcePoints(settlementPoints.at(maxIndex));
		numSettlements++;
		score++;
		return maxIndex;
	}

	return -1;
}

// Build a road for the player, if it is possible to do so
int MonteComp::placeRoad(vector<DoublePoint> roadPoints) {
	vector<int> resourcesToTrade = roadPortResources();
	// Return -1 if the player already has the maximum number of roads, 
	// or if there are no locations available for the player to build a road, 
	// or if the player does not have enough resources to build a road 
	if (numRoads >= 15 || roadPoints.size() == 0 || (resourcesToTrade.size() > 0 && resourcesToTrade[0] == -1))
		return -1;

	// Run simulations on the default option, which is not to place a road
	int maxValue = runSimulation(currentBoard, resources, score);
	int maxIndex = -1;

	// Create a copy of the player's current resources
	for (int i = 0; i < length(resources); i++) {
		tempResources[i] = resources[i];
	}

	// First make the necessary port trades to place the road and adjust temporary resources
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (tempResources[3] == 0 && resourcesToTrade.at(i) != 3)
			resourceReceived = 3;
		else if (tempResources[4] == 0 && resourcesToTrade.at(i) != 4)
			resourceReceived = 4;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade.at(i))
				<< " by player " << playerNum << " while trying to build a settlement" << endl;
		}
		tempPortResource(resourcesToTrade.at(i), resourceReceived);
	}
	tempResources[3]--;
	tempResources[4]--;
	int tempScore = score + 1;

	// Create a test board with a road in every possible location and run Monte Carlo simulations on each board
	for (unsigned int i = 0; i < roadPoints.size(); i++) {
		Board testBoard = currentBoard;
		testBoard.addRoad(roadPoints[i].getP1(), roadPoints[i].getP1(), playerNum - 1);

		int value = runSimulation(testBoard, tempResources, tempScore);
		if (value >= maxValue) {
			maxValue = value;
			maxIndex = i;
		}
	}

	// Return -1 if the simulations showed that it is better not to build a road
	if (maxIndex == -1)
		return -1;

	// Trade resources with the bank until a road can be built
	for (unsigned int i = 0; i < resourcesToTrade.size(); i++) {
		int resourceReceived = -1;
		if (resources[3] == 0 && resourcesToTrade.at(i) != 3)
			resourceReceived = 3;
		else if (resources[4] == 0 && resourcesToTrade.at(i) != 4)
			resourceReceived = 4;
		else {
			cerr << "ERROR: Invalid attempt to trade " << getResourceType(resourcesToTrade.at(i))
				<< " by player " << playerNum << " while trying to build a road" << endl;
		}
		portResource(resourcesToTrade.at(i), resourceReceived);
	}

	// Build a road at the best location for the player based on Monte Carlo simulation
	if (resources[3] > 0 && resources[4] > 0) {
		resources[3]--;
		resources[4]--;
		currentBoard.addRoad(roadPoints.at(maxIndex).getP1(), roadPoints.at(maxIndex).getP2(), playerNum);
		numRoads++;
		return maxIndex;
	}

	return -1;
}

// First build cities until no more can be built, then build settlements until no more 
// can be built, then build roads until no more can be built. Finally update the player's 
// resources within the Board class and return the updated Board to the Game class. 
Board MonteComp::takeTurn(Board b) {
	currentBoard = b;
	vector<Point> cityPoints, settlementPoints;
	vector<DoublePoint> roadPoints;
	int cityIndex, settlementIndex, roadIndex;

	while (true) {
		// Find all the locations where the player could potentially build a city
		cityPoints = currentBoard.getPossibleCityLocations(playerNum);

		// Build a city, if it is possible to do so
		cityIndex = placeCity(cityPoints);

		// Break out of the loop if it is not possible to build another city
		if (cityIndex == -1)
			break;

		// Remove the city location from the list of possible city locations
		if (cityPoints.size() != 0)
			cityPoints.erase(cityPoints.begin() + cityIndex);
	}

	while (true) {
		// Find all the locations where the player could potentially build a settlement
		settlementPoints = currentBoard.getPossibleSettlementLocations(playerNum);

		// Build a settlement, if it is possible to do so
		settlementIndex = placeSettlement(settlementPoints);

		// Break out of the loop if it is not possible to build another settlement
		if (settlementIndex == -1)
			break;

		// Remove the settlement location from the list of possible settlement locations
		if (settlementPoints.size() != 0)
			settlementPoints.erase(settlementPoints.begin() + settlementIndex);
	}

	while (true) {
		// Find all the locations where the player could potentially build a road
		roadPoints = currentBoard.getPossibleRoadLocations(playerNum);

		// Build a road, if it is possible to do so
		roadIndex = placeRoad(roadPoints);

		// Break out of the loop if it is not possible to build another road
		if (roadIndex == -1)
			break;

		// Remove the road location from the list of possible road locations
		if (roadPoints.size() != 0)
			roadPoints.erase(roadPoints.begin() + roadIndex);
	}

	// Set the board's number of resources for the player based on the player's actual resources
	currentBoard.setNumResources(playerNum, getTotalResources());

	return currentBoard;
}

int MonteComp::runSimulation(Board testBoard, int playerResources[], int playerNumPoints) {
	int playerToMove;
	if (playerNum == 4)
		playerToMove = 1;
	else
		playerToMove = playerNum + 1;
	Simulation s = Simulation(testBoard, playerResources, playerNumPoints, playerToMove);
	int value = s.runPlayouts(playerNum);
	return value;
}
