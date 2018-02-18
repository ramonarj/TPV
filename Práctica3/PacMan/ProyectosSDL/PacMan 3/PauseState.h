#pragma once
#include "GameState.h"

class PauseState:public GameState
{
private:
	PlayState* playState;
public:
	PauseState();
	PauseState(Game* juego, PlayState* playState, HUD* hud);

	virtual void render();

	//FUNCIONES DE LOS BOTONES
	static void BackToGame(Game* juego);
	static void SaveGame(Game* juego);
	static void BackToMenu(Game* juego);
};

