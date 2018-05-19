#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}

CollisionManager::CollisionManager(SDLGame* game, BulletsManager* bulletsManager, AsteroidsManager * asteroidManager, FightersManager * fightersManager, BonusManager* bonusManager)
	: GameObject(game), Observable(),  bulletsManager_(bulletsManager), asteroidsManager_(asteroidManager), fightersManager_(fightersManager), bonusManager_(bonusManager)
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
	Bonus* bonus = bonusManager_->getBonus();

	// fighter with asteroids
	if (fighter->isActive())
	{
		for (Asteroid* a : asteroids) {
			if (a->isActive()) {
				if (Collisions::collidesWithRotation(fighter, a))
				{
					AsteroidFighterCollision afc = AsteroidFighterCollision(a, fighter);
					send(&afc);
				}
			}
		}


		// bullets with asteroids
		for (Asteroid* a : asteroids) {
			if (a->isActive()) {
				for (Bullet* b : bullets) {
					if (b->isActive()) {
						if (Collisions::collides(a, b))
						{
							BulletAsteroidCollision bac = BulletAsteroidCollision(b, a);
							send(&bac);
						}
					}
				}
			}
		}

		if (bonus->isActive()) 
		{
			for (Bullet* b : bullets) {
				if(b->isActive()) {
					if (Collisions::collides(bonus, b))
					{
						BulletBonusCollision bbc = BulletBonusCollision(fighter, b);
						send(&bbc);
					}
				}
			}
		}
	}
}

void CollisionManager::render(Uint32 time)
{
}
