#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "Asteroid.h"
#include "Bullet.h"
#include "Fighter.h"

enum MessageId {
	BULLET_ASTEROID_COLLISION,
	BULLET_FIGHTER_COLLISION,
	ASTEROID_FIGHTER_COLLISION,
	FIGHTER_SHOOT,
	GAME_OVER,
	ROUND_START,
	ROUND_OVER,
	BULLET_CREATED,
	NO_MORE_ASTEROIDS,
	BADGE_ON,
	BADGE_OFF
};

struct Message {
	Message(MessageId id) :
			id_(id) {
	}
	MessageId id_;
};

struct BulletAsteroidCollision: Message {
	BulletAsteroidCollision(Bullet* bullet, Asteroid* asteroid) :
			Message(BULLET_ASTEROID_COLLISION), bullet_(bullet), asteroid_(
					asteroid) {
	}
	Bullet* bullet_;
	Asteroid* asteroid_;
};

struct BulletFighterCollision: Message {
	BulletFighterCollision(Bullet* bullet, Fighter* fighter) :
			Message(BULLET_FIGHTER_COLLISION), bullet_(bullet), fighter_(
					fighter) {
	}
	Bullet* bullet_;
	Fighter* fighter_;
};

struct AsteroidFighterCollision: Message {
	AsteroidFighterCollision(Asteroid* asteroid, Fighter* fighter) :
			Message(ASTEROID_FIGHTER_COLLISION), asteroid_(asteroid), fighter_(
					fighter) {
	}

	Asteroid* asteroid_;
	Fighter* fighter_;
};

struct FighterIsShooting: Message {
	FighterIsShooting(Fighter* fighter, Vector2D bulletPosition,
			Vector2D bulletVelocity) :
			Message(FIGHTER_SHOOT), fighter_(fighter), bulletPosition_(
					bulletPosition), bulletVelocity_(bulletVelocity) {
	}

	Fighter* fighter_;
	Vector2D bulletPosition_;
	Vector2D bulletVelocity_;
};
#endif /* MESSAGES_H_ */
