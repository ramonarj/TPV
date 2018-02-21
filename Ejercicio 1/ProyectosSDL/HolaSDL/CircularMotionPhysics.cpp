#include "CircularMotionPhysics.h"



CircularMotionPhysics::CircularMotionPhysics()
{
}

void CircularMotionPhysics::update(GameObject* o, Uint32 time) {
	o->setPosition(o->getPosition() + o->getVelocity());

	Vector2D position_ = o->getPosition();

	double x = position_.getX();
	double y = position_.getY();

	if (x >= o->getGame()->getWindowWidth()) {
		x = 0;
	}
	else if (x + o->getWidth() <= 0) {
		x = o->getGame()->getWindowWidth() - o->getWidth();
	}

	if (y >= o->getGame()->getWindowHeight()) {
		y = 0;
	}
	else if (y + o->getHeight() <= 0) {
		y = o->getGame()->getWindowHeight() - o->getHeight();
	}

	position_.setX(x);
	position_.setY(y);
	o->setPosition(position_);
}

CircularMotionPhysics::~CircularMotionPhysics()
{
}
