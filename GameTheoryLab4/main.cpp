#include <iostream>
#include <vector>
#include "NetGame.h"

using namespace std;

int main()
{
	NetGame game(0.0001, 8, -1, 1, -1, 1);
	game.generateGame();
	game.solveSimpleGame();
	game.solveBoughtGame();
	system("pause");
	return 0;
}