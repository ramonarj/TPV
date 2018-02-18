#include "PlayCont.h"
#include "EndState.h"
#include "Game.h"



PlayCont::PlayCont(): PlayState(nullptr, nullptr, ""), maxUpdates(0){}
PlayCont::PlayCont(Game* juego, HUD* hud, string filename, int cont)
: PlayState(juego, hud, filename), maxUpdates(cont){}


void PlayCont::update()
{
	if (maxUpdates == 0)
		juego->getStateMachine()->pushState(new EndState(juego, hud));
	else {
		maxUpdates--;
		PlayState::update();
	}
}

PlayCont::~PlayCont(){}
