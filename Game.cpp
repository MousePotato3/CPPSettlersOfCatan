/*******************************************************************************************
* The base class for the Settlers of Catan board game. Creates an instance of each player 
* and the Settlers of Catan board, and calls each player's methods for taking its turn. 
* Does not have access to each player's hidden resources and development cards.
*******************************************************************************************/
#include "Game.h"

template<typename T, int size>
int length(T(&)[size]) { return size; }

Game::Game() {
    // Initialize the variables for the Game class and call each player's constructor
    numPlayers = 4;
    maxResources = 7;
    pointsToWin = 10;
    playerToMove = 1;
    board.setNumPlayers(4);
    players[0] = new MonteComp(1, "red", "MonteComp", numPlayers);
    players[1] = new MonteComp(2, "blue", "MonteComp", numPlayers);
    players[2] = new RandComp(3, "white", "RandComp", numPlayers);
    players[3] = new RandComp(4, "orange", "RandComp", numPlayers);
/*  players[0] = new RandComp(1, "red", "RandComp", numPlayers);
    players[1] = new RandComp(2, "blue", "RandComp", numPlayers);
    players[2] = new RandComp(3, "white", "RandComp", numPlayers);
    players[3] = new RandComp(4, "orange", "RandComp", numPlayers);
*/  playerColors = { "red", "blue", "white", "orange" };

    // Call the methods to set up the Settlers of Catan board
    cout << "Shuffling and placing Catan tiles and ports" << endl;
    board.initTiles();
    board.initPorts();
    cout << "Finished setting up the board" << endl;
    initPlacement();
    cout << "Finished initial placements" << endl;
}

// Alternate turns between each player until some player wins, and then print the winner
void Game::play()
{
    int winner = -1;
    while (winner == -1)
        winner = takeTurn();
    cout << "Player " << winner << " won with " << players[winner - 1]->getScore() 
        << " points at the end of turn " << board.getTurnNumber() << "!" << endl;
}

// Place initial settlements and roads for each player and print the location of each settlement and
// road chosen. Also print a message for any ports acquired by players in the initial settlement phase.
void Game::initPlacement() {
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
        for(unsigned int i=0; i<neighbors.size(); i++)

        roadLocation = players[j]->chooseInitialRoadLocation(settleLocation);
        board.addRoad(settleLocation, roadLocation, playerToMove, true);
        cout << "Player " << playerToMove << " placed its second road between "
            << settleLocation.getX() << " " << settleLocation.getY() << " and "
            << roadLocation.getX() << " " << roadLocation.getY() << endl;
    }
}

void Game::collectResources() {
    // Eventually players should have an option to play knight cards before rolling dice
    int diceroll = board.rollDice();

    if (diceroll == 7) {
        // Players with more than a certain number of resources(usually half of their hand)
        // discard half of their resources, rounded down, as specified in their player class
        for (int i = 0; i < length(players); i++)
            if (players[i]->getTotalResources() > maxResources)
                players[i]->discard();

        // The player whose turn it is moves the robber and steals from another player
        int playerToRob = players[playerToMove - 1]->getPlayerToRob();
        board.setRobberLocation(players[playerToMove - 1]->getPointToBlock(playerToRob));
        int resourceNum = players[playerToRob - 1]->getRandomResource();
        if (resourceNum != -1) {
            players[playerToMove - 1]->gainResource(resourceNum);
            players[playerToRob - 1]->loseResource(resourceNum);
        }
    }
    else{
        // Each player with a settlement or city next to a resource tile with a number 
        // matching the dice roll gains resources: 1 for a settlement, or 2 for a city. 
        // These resources are not collected by the player if the tile is blocked by the robber. 
        vector<Settlement> settlements = board.getSettlements();
        vector<City> cities = board.getCities();
        for (unsigned int i = 0; i < settlements.size(); i++) {
            neighbors = board.getAdjacentHexes(settlements.at(i).getLocation());
            for (unsigned int j = 0; j < neighbors.size(); j++) {
                if (neighbors.at(j).getNumber() == diceroll && neighbors.at(j).getLocation() != board.getRobberLocation()) {
                    players[settlements.at(i).getPlayerNum() - 1]->addResource(neighbors.at(j).getType());
                }
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

int Game::takeTurn() {
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
