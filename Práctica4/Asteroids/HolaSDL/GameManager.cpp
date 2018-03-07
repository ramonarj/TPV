#include "GameManager.h"



GameManager::GameManager()
{
}

GameManager::GameManager(SDLGame * game)
{
}


GameManager::~GameManager()
{
}

bool GameManager::isGameOver() const
{
	return false;
}

int GameManager::getLives() const
{
	return 0;
}

bool GameManager::isRunning() const
{
	return false;
}

void GameManager::setRunning(bool running)
{
}

int GameManager::getScore() const
{
	return 0;
}

void GameManager::setBadge(bool b)
{
}

void GameManager::receive(Message * msg)
{
	/*switch (msg->id_) {
	case ASTROID_FIGHTER_COLLISION:
		…
			break;
	case BULLET_ASTEROID_COLLISION:
		…
			break;
	case NO_MORE_ATROIDS:
		…
			break;
		…
	}*/
}
