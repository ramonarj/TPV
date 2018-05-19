#pragma once

#include "Container.h"


/*
 *
 */
class Bullet: public Container {
public:
	Bullet(SDLGame* game);
	virtual ~Bullet();
	void setFighterId(Uint8 id);
	Uint8 getFighterId();
	void setBulletId(Uint32 id);
	Uint32 getBulletId_();
private:
	Uint8 fighterId_;
	Uint32 bulletId_;
};

