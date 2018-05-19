#include "CollisionManager.h"
#include "Collisions.h"

CollisionManager::CollisionManager(SDLGame* game,
		BulletsManager* bulletsManager, FightersManager* fightersManager,
		AsteroidsManager* asteroidsManager) :
		GameObject(game), bulletsManager_(bulletsManager), fightersManager_(
				fightersManager), asteroidsManager_(asteroidsManager) {
}

CollisionManager::~CollisionManager() {
}

void CollisionManager::handleInput(Uint32 time, const SDL_Event& event) {
}

void CollisionManager::update(Uint32 time) {

	// only the master client checks collisions
	if ( !getGame()->isMasterClient() )
		return;

	vector<Bullet*> bullets = bulletsManager_->getBullets();
	vector<Fighter*> fighters = fightersManager_->getFighters();
	vector<Asteroid*> asteroids = asteroidsManager_->getAsteroids();

	for (Bullet* b : bullets) {
		if (b->isActive()) {
			for (Fighter* f : fighters) {
				if (f != nullptr && f->isActive()
						&& Collisions::collidesWithRotation(b, f)) {
					BulletFighterCollisionMsg m = { f->getId(), b->getBulletId_() };
					send(&m);
					break; // it can kill only one fighter
				}
			}

			for (Asteroid* a : asteroids) {
				if (a->isActive() && Collisions::collidesWithRotation(b, a)) {
					BulletAsteroidCollisionMsg m = { b->getBulletId_() };
					send(&m);
					break;
				}
			}
		}
	}

	for (Asteroid* a : asteroids) {
		for (Fighter* f : fighters) {
			if (f != nullptr && f->isActive() && Collisions::collidesWithRotation(a, f)) {
				FighterAsteroidCollisionMsg m = { f->getId() };
				send(&m);
				break; // it can kill only one fighter
			}
		}
	}
}

void CollisionManager::render(Uint32 time) {
}
