#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(SDLGame * game): Container(game)
{
}


Bullet::~Bullet()
{
}

void Bullet::setFighterId(int id)
{
	fighterId_ = id;
}

int Bullet::getFighterId()
{
	return fighterId_;
}
