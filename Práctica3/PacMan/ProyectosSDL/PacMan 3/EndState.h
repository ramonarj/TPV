#pragma once
#include "GameState.h"
using namespace std;

class EndState:public GameState
{
private:
	
	
public:
	EndState();
	EndState(Game* juego, HUD* hud);
	virtual void render();

	//FUNCIONES DE LOS BOTONES
	static void BackToMenu(Game* juego);
	static void ExitGame(Game* juego);
};

