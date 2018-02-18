#pragma once
#include "HUD.h"
#include "GameState.h"

class MainMenuState: public GameState
{
public:

	virtual void render();
	MainMenuState();
	MainMenuState(Game*juego, HUD* hud);

	//Funciones de los botones
	static void playGame(Game* juego);
	static void loadGame(Game* juego);
	static void exitGame(Game* juego);
};



