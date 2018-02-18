#pragma once
#include "GameObject.h"


class Game;
class PlayState;
class PacManObject: public GameObject
{
protected:

	PlayState* playState;
	PacManObject();
	PacManObject(Game* juego, PlayState* playState);

public:
	virtual void loadFromFile(ifstream& file) = 0;
	virtual void saveToFile(ofstream& file) = 0;
	virtual ~PacManObject();
};

