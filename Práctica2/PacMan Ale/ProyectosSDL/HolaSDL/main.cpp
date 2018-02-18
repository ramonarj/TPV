//Alejandro Villar Rubio
//Ramón Arjona Quiñones
//Pac-Man 2.0, 1º de TPV, Grado en Desarrollo de Vidojuegos, UCM
#include "SDL.h"
#include <iostream>
#include "Game.h"


using namespace std;

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game juego;
	juego.Run();

	return 0;
}