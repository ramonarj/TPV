#pragma once

#include "BulletsManager.h"
#include "AsteroidsManager.h"
#include "FightersManager.h"
#include "BonusManager.h"
#include "Collisions.h"

class CollisionManager : public GameObject, public Observable {
public:
	CollisionManager();
	CollisionManager(SDLGame* game,
		BulletsManager* bulletsManager,
		AsteroidsManager* asteroidManager,
		FightersManager* fightersManager,
		BonusManager* bonusManager);
	virtual ~CollisionManager();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

private:
	BulletsManager* bulletsManager_;
	AsteroidsManager* asteroidsManager_;
	FightersManager* fightersManager_;
	BonusManager* bonusManager_;
};

