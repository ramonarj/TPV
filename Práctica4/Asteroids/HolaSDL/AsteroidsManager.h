#pragma once
#include "GameObject.h"
#include "Observer.h"
#include "Observable.h"
#include "ImageRenderer.h"
#include "CircularMotionPhysics.h"
#include "RotationPhysics.h"
#include "SkeletonRenderer.h"
#include <algorithm>

class AsteroidsManager : public GameObject, public Observer, public Observable{
public:
	AsteroidsManager();
	AsteroidsManager(SDLGame* game);
	virtual ~AsteroidsManager();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

	virtual vector<Asteroid*>& getAsteroids();
	virtual void receive(Message* msg);

private:
	Asteroid* getAsteroid();
	void initAsteroids();
	void createBabyAsteroid(Asteroid* a);

	vector<Asteroid*> asteroids_;

	ImageRenderer* asteroidImage_;
	CircularMotionPhysics* circularPhysics_;
	RotationPhysics* rotationPhysics_;
	SkeletonRendered* skeRender_;

	int numOfAsteroids_;
};

