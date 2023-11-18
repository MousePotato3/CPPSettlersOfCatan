#include "Game.h"
#include <iostream>
#include <stdlib.h>
using namespace std;
int main( int argc, char* args[] ) 
{ 
    Game game;
	cout << "About to play the game Setters of Catan" << endl;
    game.play();
    cout << "Done playing the game Setters of Catan" << endl;
    cin.ignore(256, '\n');
    cout << "Press ENTER to continue..." << endl;
    cin.get();
    return 0; 
}

