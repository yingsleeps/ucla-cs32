
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>


#include "Gark.h"
#include "Player.h"
#include "Mesa.h"
#include "Game.h"

using namespace std;

int main()
{
      // Create a game
//     Use this instead to create a mini-game:
    Game g(4, 3, 4);
//    Game g(7, 8, 25);

       //Play the game
    g.play();

}

