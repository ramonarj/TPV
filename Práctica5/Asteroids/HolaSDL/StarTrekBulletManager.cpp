#include "StarTrekBulletManager.h"



StarTrekBulletManager::StarTrekBulletManager(): GameObject(), BulletsManager()
{
}

StarTrekBulletManager::StarTrekBulletManager(SDLGame * game): GameObject(game), BulletsManager()
{
	bulletPhysics_ = BasicMotionPhysics();
	bulletRenderer_ = FillRectRenderer({ 255, 0, 0, 255 });
}

StarTrekBulletManager::~StarTrekBulletManager()
{
	for (int i = 0; i < bullets_.size(); i++)
		delete bullets_[i];
}

void StarTrekBulletManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void StarTrekBulletManager::update(Uint32 time)
{
	for (Bullet* b: bullets_)
	{
		if (b->isActive())
			b->update(time);
	}
}

void StarTrekBulletManager::render(Uint32 time)
{
	for (Bullet* b : bullets_)
	{
		if (b->isActive())
			b->render(time);
	}
}

Bullet * StarTrekBulletManager::getBullet()
{
	int i = 0;
	while (i < bullets_.size() && bullets_.at(i)->isActive())
		i++;

	if (i == bullets_.size())
	{
		Bullet* b = new Bullet(game_);
		b->addRenderComponent(&bulletRenderer_);
		b->addPhysicsComponent(&bulletPhysics_);
		bullets_.push_back(b);
		return b;
	}
	else
	{
		return bullets_.at(i);
	}
}

void StarTrekBulletManager::receive(Message * msg)
{
	switch (msg->id_) {
		case ROUND_START:case ROUND_OVER:
		{		
			for (Bullet* b : bullets_)
				b->setActive(false);
			break;
		}

		case BULLET_ASTEROID_COLLISION: case BULLET_FIGHTER_COLLISION:
		{
			Bullet* b = static_cast<BulletAsteroidCollision*>(msg)->bullet_;
			if (!superBadge_) 
				b->setActive(false);
			break;
		}

		case FIGHTER_SHOOT:
		{
			Fighter* f = static_cast<FighterIsShooting*>(msg)->fighter_;
			Vector2D p = static_cast<FighterIsShooting*>(msg)->bulletPosition_;
			Vector2D v = static_cast<FighterIsShooting*>(msg)->bulletVelocity_;
			shoot(f, p, v);
			break;
		}
		case NORMAL_BADGE:
		{
			normalBadge_ = true;
			break;
		}
		case SUPER_BADGE:
		{
			normalBadge_ = false;
			superBadge_ = true;
			break;
		}
		case MULTI_BADGE:
		{
			normalBadge_ = false;
			superBadge_ = false;
			multiBadge_ = true;
			break;
		}
		case BADGE_OFF:
		{
			normalBadge_ = false;
			superBadge_ = false;
			multiBadge_ = false;
			break;
		}
	}
}

void StarTrekBulletManager::shoot(Fighter* owner, Vector2D position, Vector2D velocity)
{
	if (!multiBadge_)
	{
		Bullet* b = getBullet();
		b->setPosition(position);
		b->setVelocity(velocity);
		b->setWidth(3);
		b->setHeight(3);
		b->setActive(true);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			Bullet* b = getBullet();
			b->setPosition(position);
			velocity.rotate(60);
			b->setVelocity(velocity);
			b->setWidth(3);
			b->setHeight(3);
			b->setActive(true);
		}
	}

	send(&Message(BULLET_CREATED));
}
