#include "RotationPhysics.h"



RotationPhysics::RotationPhysics()
{
	velRotation_ = rand() % 3;
	int posNeg = rand() & 2;
	if (posNeg == 0) velRotation_ *= -1;
}


RotationPhysics::~RotationPhysics()
{
}

void RotationPhysics::update(GameObject * o, Uint32 time)
{
	Vector2D direction = o->getDirection();

	direction.set(direction.getX() + velRotation_, direction.getY() + velRotation_);

	o->setDirection(direction);
}

