#pragma once
#include "GameObject.h"
#include "BulletsManager.h"
#include "GameComponent.h"
#include "FillRectRenderer.h"
#include "BasicMotionPhysics.h"

class StarWarsBulletsManager : public GameObject, public BulletsManager
{
public:
	StarWarsBulletsManager(SDLGame* game);
	~StarWarsBulletsManager();

	virtual void shoot(GameObject* o, Vector2D p, Vector2D v);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

private:
	std::vector<GameObject*> bullets;
	SDLGame* game;

	GameComponent* demoComp_;
	FillRectRenderer* fillComp_;
	BasicMotionPhysics* motionComp_;
};

