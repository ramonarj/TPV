#pragma once
#include "Container.h"
class Bullet : public Container
{
public:
	Bullet();
	Bullet(SDLGame* game);
	virtual ~Bullet();

	void setFighterId(int id);
	int getFighterId();

private:
	int fighterId_;
};

