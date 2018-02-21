#include "RotationInputComponent.h"



RotationInputComponent::RotationInputComponent(double angle, SDL_Keycode clockwise, SDL_Keycode counter_clockwise): angle(angle), clockwise(clockwise), counter_clockwise(counter_clockwise)
{
}

void RotationInputComponent:: rotation(GameObject* o, Uint32 time,
	const SDL_Event& event)
{
	Vector2D direction = o->getDirection();

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == clockwise) {
			direction.rotate(angle);
		}
		else if (event.key.keysym.sym == counter_clockwise) {
			direction.rotate(-angle);
		}
	}

	o->setDirection(direction);
}

RotationInputComponent::~RotationInputComponent()
{
}
