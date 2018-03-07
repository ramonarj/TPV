#include "StarTrekBulletManager.h"



StarTrekBulletManager::StarTrekBulletManager()
{
}

StarTrekBulletManager::StarTrekBulletManager(SDLGame * game)
{
}


StarTrekBulletManager::~StarTrekBulletManager()
{
}

void StarTrekBulletManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void StarTrekBulletManager::update(Uint32 time)
{
}

void StarTrekBulletManager::render(Uint32 time)
{
}

void StarTrekBulletManager::receive(Message * msg)
{
	/*switch (msg->id_) {
	case ROUND_START:
		…
			break;
	case ROUND_OVER:
		…
			break;
	case BULLET_ASTEROID_COLLISION:
		…
			break;
	case BULLET_FIGHTER_COLLISION:
		…
			break;
	case FIGHTER_SHOOT:
		…
			break;
		…
	}*/
}

void StarTrekBulletManager::shoot(Fighter * owner, Vector2D position, Vector2D velocity)
{
}
