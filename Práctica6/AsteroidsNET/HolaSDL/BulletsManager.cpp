#include "BulletsManager.h"

#include "FillRectRenderer.h"
#include "BasicMotionPhysics.h"
#include "Container.h"

BulletsManager::BulletsManager(SDLGame* game) :
		GameObject(game), bulllets_(), bulletRenderer_( { COLOR(0xffffffff) }), basicMotionPhysics_(), deactiveOnExitBordersPhysics_() {
}

BulletsManager::~BulletsManager() {
}

void BulletsManager::shoot(Uint8 fighterId_, Vector2D position,
		Vector2D velocity) {
	Bullet* b = getBullet();
	b->setActive(true);
	b->setPosition(position);
	b->setVelocity(velocity);
	b->setDirection(velocity);
}

void BulletsManager::handleInput(Uint32 time, const SDL_Event& event) {
}

void BulletsManager::update(Uint32 time) {
	for (Bullet* b : bulllets_) {
		if (b->isActive()) {
			b->update(time);
		}
	}
}

void BulletsManager::render(Uint32 time) {
	for (Bullet* b : bulllets_) {
		if (b->isActive()) {
			b->render(time);
		}
	}
}

std::vector<Bullet*>& BulletsManager::getBullets() {
	return bulllets_;
}

Bullet* BulletsManager::getBullet() {

	// look for an inactive bullet
	vector<Bullet*>::iterator it = bulllets_.begin();
	while (it != bulllets_.end() && *it != nullptr && (*it)->isActive())
		it++;

	Bullet* b = nullptr;
	if (it != bulllets_.end()) {
		b = *it;
	} else { // if no inactive bullet, create one;
		b = new Bullet(getGame());
		b->addPhysicsComponent(&basicMotionPhysics_);
		b->addPhysicsComponent(&deactiveOnExitBordersPhysics_);
		b->addRenderComponent(&bulletRenderer_);
		b->setWidth(2);
		b->setHeight(2);

		// the if of the bullet is its position in the vector
		b->setBulletId(bulllets_.size());

		bulllets_.push_back(b);
	}

	return b;

}

void BulletsManager::disableBullets() {
	for (Bullet* b : bulllets_) {
		b->setActive(false);
	}
}

void BulletsManager::disableBullet(Uint16 id) {
	bulllets_[id]->setActive(false);
}

void BulletsManager::receive(Message* msg) {
	switch (msg->mType_) {
		case GAME_OVER:
			disableBullets();
			break;
		case FIGHTER_SHOOT: {
			FighterIsShootingMsg* m = static_cast<FighterIsShootingMsg*>(msg);
			shoot(m->fighterId_, m->bulletPosition_, m->bulletVelocity_);
			break;
		}
		case BULLET_FIGHTER_COLLISION: {
			disableBullet(static_cast<BulletFighterCollisionMsg*>(msg)->bulletId_);
			break;
		}
		case BULLET_ASTEROID_COLLISION: {
			disableBullet(static_cast<BulletAsteroidCollisionMsg*>(msg)->bulletId_);
		}
									  
	}
}

