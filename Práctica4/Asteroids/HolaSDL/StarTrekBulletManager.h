#pragma once

#include "BulletsManager.h"
#include "FillRectRenderer.h"
#include "BasicMotionPhysics.h"

class StarTrekBulletManager : public GameObject, public BulletsManager {
public:
	StarTrekBulletManager();
	StarTrekBulletManager(SDLGame* game);
	virtual ~StarTrekBulletManager();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

	virtual vector<Bullet*>& getBullets() { return bullets_; };

private:
	Bullet* getBullet();
	virtual void receive(Message* msg);
	virtual void shoot(Fighter* owner, Vector2D position, Vector2D velocity);

	vector<Bullet*> bullets_;
	FillRectRenderer bulletRenderer_;
	BasicMotionPhysics bulletPhysics_;
};;

