#pragma once

#include <vector>
#include "FillRectRenderer.h"
#include "BasicMotionPhysics.h"
#include "Bullet.h"
#include "Observer.h"
#include "Observable.h"
#include "DeActivateOnBorderExit.h"

/*
 *
 */
class BulletsManager: public GameObject, public Observable, public Observer {
public:
	BulletsManager(SDLGame* game);
	virtual ~BulletsManager();


	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

	virtual std::vector<Bullet*>& getBullets();

private:
	Bullet* getBullet();
	virtual void receive(Message* msg);
	virtual void shoot(Uint8 fighterId_, Vector2D position, Vector2D velocity);
	void disableBullets();
	void disableBullet(Uint16 id);

	vector<Bullet*> bulllets_;
	FillRectRenderer bulletRenderer_;
	BasicMotionPhysics basicMotionPhysics_;
	DeActivateOnBorderExit deactiveOnExitBordersPhysics_;

};

