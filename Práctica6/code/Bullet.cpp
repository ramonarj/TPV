#include "Bullet.h"

Bullet::Bullet(SDLGame* game) :
		Container(game), fighterId_(-1), bulletId_(-1) {
}

Bullet::~Bullet() {
}

void Bullet::setFighterId(Uint8 id) {
	fighterId_ = id;
}

Uint8 Bullet::getFighterId() {
	return fighterId_;
}

void Bullet::setBulletId(Uint32 id) {
	bulletId_ = id;
}

Uint32 Bullet::getBulletId_() {
	return bulletId_;
}
