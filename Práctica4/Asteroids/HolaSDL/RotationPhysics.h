#pragma once
#include "PhysicsComponent.h"
class RotationPhysics : public PhysicsComponent
{
public:
	RotationPhysics();
	~RotationPhysics();

	virtual void update(GameObject* o, Uint32 time);

private:
	int velRotation_;
};

