#include "FightersManager.h"



FightersManager::FightersManager()
{
}

FightersManager::FightersManager(SDLGame * game, Observer * bulletsMamager)
{
}


FightersManager::~FightersManager()
{
}

void FightersManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void FightersManager::update(Uint32 time)
{
}

void FightersManager::render(Uint32 time)
{
}

void FightersManager::receive(Message * msg)
{
	/*switch (msg->id_) {
	case ROUND_START:
		…
			break;
	case ROUND_OVER:
		…
			break;
	case BADGE_ON:
		…
			break;
	case BADGE_OFF:
		…
			break;
		…
	}*/
}
