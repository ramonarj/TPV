#pragma once
#include "PhysicsComponent.h"

class DestroyWithBorder : public PhysicsComponent
{
public:
	DestroyWithBorder();
	~DestroyWithBorder();

	void update(GameObject *o, Uint32 time);
};

