#include "AccelerationInputComponent.h"


AccelerationInputComponent::AccelerationInputComponent(double thrust, double maxVel, SDL_Keycode ac, SDL_Keycode des)
	: thrust(thrust), maxVel(maxVel), ac(ac), des(des) {}


AccelerationInputComponent::~AccelerationInputComponent(){}

void AccelerationInputComponent:: acceleration(GameObject * o, Uint32 time, const SDL_Event & event)
{
	Vector2D velocity_ = o->getVelocity();

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == ac) {
			if (velocity_.magnitude() >= maxVel)
			{
				velocity_.normalize();
				velocity_ = velocity_ * maxVel;
			}
			else
				velocity_ = velocity_ + o->getDirection() * thrust;
		}
		else if (event.key.keysym.sym == des) {
			if (velocity_.magnitude() >= C > 0)
				velocity_ = velocity_ * ((velocity_.magnitude() - C) / velocity_.magnitude());
			else
				velocity_.set(0, 0);
		}
	}

		o->setVelocity(velocity_);
}
