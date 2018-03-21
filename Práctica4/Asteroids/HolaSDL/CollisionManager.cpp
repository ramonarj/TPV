#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}

CollisionManager::CollisionManager(SDLGame* game, BulletsManager* bulletsManager, AsteroidsManager * asteroidManager, FightersManager * fightersManager)
	: GameObject(game), Observable(),  bulletsManager_(bulletsManager), asteroidsManager_(asteroidManager), fightersManager_(fightersManager)
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
	for (Asteroid* a : asteroids) {
		if (a->isActive()) {
			if (Collisions::collidesWithRotation(fighter, a))
				send(ASTEROID_FIGHTER_COLLISION);
		}
	}

	// bullets with asteroids
	for (Asteroid* a : asteroids) {
		if (a->isActive()) {
			for (Bullet* b : bullets)
				if (Collisions::collides(a, b))
					send(BULLET_ASTEROID_COLLISION);
		}
	}
}

void CollisionManager::render(Uint32 time)
{
}
