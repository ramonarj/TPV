#pragma once
#include "PhysicsComponent.h"

class CircularMotionPhysics: public PhysicsComponent
{
public:
	CircularMotionPhysics();
	~CircularMotionPhysics();
	virtual void update(GameObject* o, Uint32 time);
};

