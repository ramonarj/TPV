#pragma once
#include "GameObject.h"
#include <list>
#include "HUD.h"

class Game;
class GameState
{
protected:
	Game* juego;
	list<GameObject*> stage;
	HUD* hud;

	GameState();
	GameState(Game* juego, HUD* hud);
public:
	virtual void render();
	virtual void update();
	virtual void handleEvent(SDL_Event &e);
	virtual ~GameState();
};

