
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "Game.h"
using namespace std;


int main(int argc, char* argv[]){
	Game* juego = new Game();
	juego->run();

	
	system("pause");

	return 0;
}