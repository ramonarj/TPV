#pragma once
#include "PlayState.h"

class PlayCont: public PlayState
{
protected:
	int maxUpdates;
public:
	PlayCont();
	PlayCont(Game* juego, HUD* hud, string filename, int cont);

	virtual void update();

	virtual ~PlayCont();
};

