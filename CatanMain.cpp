/*******************************************************************************************
* The main program for the Settlers of Catan board game
*******************************************************************************************/

#include <iostream>
#include <stdlib.h>
#include "Game.h"
using namespace std;

int main( int argc, char* args[] ) 
{
    // Create a new instance of the Game class and call its main method
    Game game;
	cout << "About to play the game Setters of Catan" << endl;
    game.play();
    cout << "Done playing the game Setters of Catan" << endl;
    cin.ignore(256, '\n');
    cout << "Press ENTER to continue..." << endl;
    cin.get();
    return 0; 
}
