#include "DestroyWithBorder.h"



DestroyWithBorder::DestroyWithBorder()
{
}


DestroyWithBorder::~DestroyWithBorder()
{
}

void DestroyWithBorder::update(GameObject * o, Uint32 time)
{
	if (o->isActive()) {
		Vector2D p = o->getPosition() + o->getVelocity();

		if (p.getX() >= o->getGame()->getWindowWidth() || p.getX() + o->getWidth() <= 0 ||
			p.getY() >= o->getGame()->getWindowHeight() || p.getY() + o->getHeight() <= 0) 
		{
			o->setActive(false);
		}

		o->setPosition(p);
	}
}
