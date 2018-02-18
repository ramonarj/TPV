#include "GameState.h"


GameState::GameState(): juego(nullptr), hud (nullptr){}
GameState::GameState(Game*juego, HUD* hud) : juego(juego), hud(hud){}

void GameState::render()
{
	for (GameObject* o : stage)
		o->render();
}

void GameState::update()
{
	for (GameObject* o : stage)
		o->update();
}

void GameState::handleEvent(SDL_Event &e)
{
	bool handled = false;
	auto it = stage.begin();
	//Le mandamos el evento al personaje del estado que corresponda
	while (!handled && it != stage.end())
	{
		if ((*it)->handleEvent(e))
			handled = true;
		else
			it++;
	}
}

GameState::~GameState()
{
	for (GameObject* o : stage)
		delete o;
}

