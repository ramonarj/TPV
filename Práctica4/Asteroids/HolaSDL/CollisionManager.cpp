#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}

CollisionManager::CollisionManager(SDLGame * game, BulletsManager * bulletsManager, AsteroidsManager * astroidManager, FightersManager * fightersManager)
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void CollisionManager::update(Uint32 time)
{
	vector<Bullet*> bullets = bulletsManager_->getBullets();
	vector<Asteroid*> asteroids = asteroidsManager_->getAsteroids();
	Fighter* fighter = fightersManager_->getFighter();

	// fighter with asteroids
	// bullets with asteroids
}

void CollisionManager::render(Uint32 time)
{
}
