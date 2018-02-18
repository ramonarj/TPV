#pragma once
#include <fstream>
#include "checkML.h"
#include "SDL.h"

using namespace std;

class Game;
class GameObject
{
protected:
	Game* juego;
	GameObject();
	GameObject(Game*juego);

public:
	//Métodos abstractos que heredarán GameMap y GameCharacter
	virtual void render() = 0;
	virtual void update () = 0;
	virtual bool handleEvent(SDL_Event& e) = 0;
	virtual ~GameObject();
};

