/*******************************************************************************************
* A modified version of the Game class used to run Monte Carlo simulations for the 
* Monte Carlo player type (MonteComp) in order to determine which move is best
*******************************************************************************************/
#include "Simulation.h"

template<typename T, int size>
int length(T(&)[size]) { return size; }

const int NUM_PLAYOUTS = 3;
const int MAX_TURNS = 100;

Simulation::Simulation(Board b, int playerResources[], int playerScore, int montePlayer) {
    // Initialize the variables for the Simulation class and call each player's constructor
	numPlayers = 4;
	maxResources = 7;
	pointsToWin = 10;
	playerToMove = 1;
	board = b;
    players[0] = new RandComp(1, "red", "RandComp", numPlayers, false);
    players[1] = new RandComp(2, "blue", "RandComp", numPlayers, false);
    players[2] = new RandComp(3, "white", "RandComp", numPlayers, false);
    players[3] = new RandComp(4, "orange", "RandComp", numPlayers, false);
	board.setVisible(false);

    // Copy the resources for the player that is running the simulation 
    // and generate the resources for the other players randomly 
    for (int i = 0; i < numPlayers; i++) {
        if (montePlayer == i + 1) {
            for (int j = 0; j < length(resources[i]); j++)
                resources[i][j] = playerResources[i];
        }
        else
            generateRandomResources(i + 1, board.getNumResources(i + 1));
    }
    // Copy the known score for the player that is running the simulation and copy 
    // the remaining scores from the Board class (ignores hidden victory point cards) 
    for (int i = 0; i < numPlayers; i++) {
        if (montePlayer == i + 1)
            scores[i] = playerScore;
        else
            scores[i] = board.getPlayerScore(i + 1);
    }

    playerToMove = montePlayer;

    // Advance to the next player's turn
    if (playerToMove == numPlayers) {
        playerToMove = 1;
        board.setTurnNumber(board.getTurnNumber() + 1);
    }
    else
        playerToMove++;
}

// Copy the board and run Monte Carlo Simulations for the MonteComp player.
// Return the number of games won by the MonteComp player. 
int Simulation::runPlayouts(int montePlayer) {
	Board startBoard = board;
    int startTurn = board.getTurnNumber();
    int value = 0;
    int unfinishedGames = 0;

    cout << "About to run " << NUM_PLAYOUTS << " simulations" << endl;

    // Run Monte Carlo simulations for the MonteComp player (runs faster in parallel)
    for (unsigned int i = 0; i < NUM_PLAYOUTS; i++) {
        int winner = -1;
        board = startBoard;
		board.resetDice();
        for (int j = 0; j < numPlayers; j++) {
			players[j]->resetData();
			players[j]->setResources(resources[j]);
			players[j]->setScore(scores[j]);

            // Generate random resources for players with unknown resources
            if (montePlayer != j + 1)
                generateRandomResources(j + 1, board.getNumResources(j + 1));
		}

        // Alternate turns between each player until some player wins, and then return the winner
        while (winner == -1)
            winner = takeTurn();
        if (winner == montePlayer)
            value++;
        else if (winner == 0)
			unfinishedGames++;
	}
    cout << "The number of wins by player " << montePlayer << " was " << value << endl;
    if (unfinishedGames > 0)
        cout << unfinishedGames << " games were not won within " << MAX_TURNS << " turns by any player" << endl;
	return value;
}

// Generate the resources for a player randomly based the player's number of resources
void Simulation::generateRandomResources(int playerNum, int numResources) {
    int randomNumber = -1;
    for (int i = 0; i < numResources; i++) {
        randomNumber = rand() * 5;
        players[playerNum - 1]->gainResource(randomNumber);
    }
}

// Place initial settlements and roads for each player and print the location of each settlement and
// road chosen. Also print a message for any ports acquired by players in the initial settlement phase.
void Simulation::initPlacement() {
    Point settleLocation, roadLocation;

    // Player 1 chooses its first settlement first
    for (int i = 0; i < numPlayers; i++) {
        playerToMove = i + 1;
        settleLocation = players[i]->chooseInitialSettlementLocation(board);
        board.addSettlement(settleLocation, playerToMove, true);
        players[i]->updateResourcePoints(settleLocation);
        players[i]->setScore(players[i]->getScore() + 1);
        cout << "Player " << playerToMove << " placed its first settlement at "
            << settleLocation.getX() << " " << settleLocation.getY() << endl;
        string newPortType = board.getPortType(settleLocation);
        if (newPortType != "") {
            players[i]->gainPortPower(newPortType);
            cout << "Player " << playerToMove << " just acquired a " << newPortType << " port!" << endl;
        }

        roadLocation = players[i]->chooseInitialRoadLocation(settleLocation);
        board.addRoad(settleLocation, roadLocation, playerToMove, true);
        cout << "Player " << playerToMove << " placed its first road between "
            << settleLocation.getX() << " " << settleLocation.getY() << " and "
            << roadLocation.getX() << " " << roadLocation.getY() << endl;
    }

    // Player 1 chooses its second settlement last 
    // Each player gains resources based on the tiles adjacent to their second settlement 
    for (int i = 0; i < numPlayers; i++) {
        int j = numPlayers - i - 1;
        playerToMove = j + 1;
        settleLocation = players[j]->chooseInitialSettlementLocation(board);
        board.addSettlement(settleLocation, playerToMove, true);
        players[j]->updateResourcePoints(settleLocation);
        players[j]->setScore(players[j]->getScore() + 1);
        cout << "Player " << playerToMove << " placed its second settlement at "
            << settleLocation.getX() << " " << settleLocation.getY() << endl;
        string newPortType = board.getPortType(settleLocation);
        if (newPortType != "") {
            players[j]->gainPortPower(newPortType);
            cout << "Player " << playerToMove << " just acquired a " << newPortType << " port!" << endl;
        }

        // Collect resources based on tiles adjacent to the player's second settlement
        neighbors = board.getAdjacentHexes(settleLocation);
        for (unsigned int i = 0; i < neighbors.size(); i++)

            roadLocation = players[j]->chooseInitialRoadLocation(settleLocation);
        board.addRoad(settleLocation, roadLocation, playerToMove, true);
        cout << "Player " << playerToMove << " placed its second road between "
            << settleLocation.getX() << " " << settleLocation.getY() << " and "
            << roadLocation.getX() << " " << roadLocation.getY() << endl;
    }
}

void Simulation::collectResources() {
    // Eventually players should have an option to play knight cards before rolling dice
    int diceroll = board.rollDice();
    // cout << diceroll << " rolled on turn " << turn << endl;

    if (diceroll == 7) {
        // Players with more than a certain number of resources(usually half of their hand)
        // discard half of their resources, rounded down, as specified in their player class
        for (int i = 0; i < length(players); i++)
            if (players[i]->getTotalResources() > maxResources)
                players[i]->discard();

        // The player whose turn it is moves the robber and steals from another player
        int playerToRob = players[playerToMove - 1]->getPlayerToRob();
        board.setRobberLocation(players[playerToMove - 1]->getPointToBlock(playerToRob));
        int resourceNum = players[playerToRob]->getRandomResource();
        if (resourceNum != -1) {
            players[playerToMove - 1]->gainResource(resourceNum);
            players[playerToRob - 1]->loseResource(resourceNum);
        }
    }
    else {
        // Each player with a settlement or city next to a resource tile with a number 
        // matching the dice roll gains resources: 1 for a settlement, or 2 for a city. 
        // These resources are not collected by the player if the tile is blocked by the robber. 
        vector<Settlement> settlements = board.getSettlements();
        vector<City> cities = board.getCities();
        for (unsigned int i = 0; i < settlements.size(); i++) {
            neighbors = board.getAdjacentHexes(settlements.at(i).getLocation());
            for (unsigned int j = 0; j < neighbors.size(); j++) {
                if (neighbors.at(j).getNumber() == diceroll)
                    players[settlements.at(i).getPlayerNum() - 1]->addResource(neighbors.at(j).getType());
            }
        }
        for (unsigned int i = 0; i < cities.size(); i++) {
            neighbors = board.getAdjacentHexes(cities.at(i).getLocation());
            for (unsigned int j = 0; j < neighbors.size(); j++) {
                if (neighbors.at(j).getNumber() == diceroll) {
                    players[cities.at(i).getPlayerNum() - 1]->addResource(neighbors.at(j).getType());
                    players[cities.at(i).getPlayerNum() - 1]->addResource(neighbors.at(j).getType());
                }
            }
        }
    }
//  for (int i = 0; i < length(players); i++)
//      players[i]->printResources();
}

int Simulation::takeTurn() {
    // Roll the dice, collect resources, and take the current player's turn
    collectResources();
    board = players[playerToMove - 1]->takeTurn(board);

    // Check to see if the player won, and if so, update the winner
    if (players[playerToMove - 1]->getScore() >= pointsToWin)
        board.setWinner(playerToMove);

    // Advance to the next player's turn
    if (playerToMove == numPlayers) {
        playerToMove = 1;
        board.setTurnNumber(board.getTurnNumber() + 1);
    }
    else
        playerToMove++;

    return board.getWinner();
}
