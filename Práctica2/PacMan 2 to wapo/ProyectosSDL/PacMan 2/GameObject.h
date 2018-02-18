#pragma once
#include <fstream>
#include "checkML.h"

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
	virtual void loadFromFile(ifstream& file) = 0;
	virtual void saveToFile(ofstream& file) = 0;
	virtual ~GameObject();
};

