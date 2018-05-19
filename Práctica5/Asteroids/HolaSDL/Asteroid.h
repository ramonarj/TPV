#pragma once
#include "Container.h"

class Asteroid : public Container {
public:

	Asteroid();
	Asteroid(SDLGame* game);
	virtual ~Asteroid();

	virtual void setGenerations(int generations);
	virtual int getGenerations();

private:
	int generations_;
};

