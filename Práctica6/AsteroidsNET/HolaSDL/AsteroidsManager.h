#pragma once

#include "GameObject.h"
#include "ImageRenderer.h"
#include "CircularMotionPhysics.h"
#include "RotationPhysics.h"
#include "Observer.h"
#include "SkeletonRenderer.h"
#include "Observable.h"
#include "Asteroid.h"

#include "ImageRenderer.h"
#include "DestroyWithBorder.h"
#include "RotationPhysics.h"
#include "CircularMotionPhysics.h"

/*
 *
 */
class AsteroidsManager: public GameObject, public Observer, public Observable {

public:
	AsteroidsManager(SDLGame* game);
	virtual ~AsteroidsManager();

	virtual vector<Asteroid*>& getAsteroids();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);
	virtual void receive(Message* msg);

	void createAsteroidMessage(Message* msg);

	Asteroid* getAsteroid();

	ImageRenderer asteroidImage_;
	//CircularMotionPhysics circularPhysics_;
	DestroyWithBorder destroyBorderPhysics_;
	RotationPhysics rotationPhysics_;

private:

	vector<Asteroid*> asteroids_;

	bool createAsteroids_;
	Uint32 createAsteroidsOn_;
	Uint32 createAsteroidsTime_;

};

