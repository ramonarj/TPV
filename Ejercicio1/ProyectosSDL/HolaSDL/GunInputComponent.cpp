#include "GunInputComponent.h"
#include <algorithm>


GunInputComponent::GunInputComponent(BulletsManager* bulletMan_, SDL_Keycode shootKey)
	:bulletMan_(bulletMan_), shootKey(shootKey)
{
}


GunInputComponent::~GunInputComponent()
{
}

void GunInputComponent::handleInput(GameObject* o, Uint32 time,
	const SDL_Event& event) {

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == shootKey) {

			Vector2D p(o->getPosition().getX() + (o->getWidth() / 2),
				o->getPosition().getY() + (o->getHeight() / 2));

			Vector2D v(o->getDirection() * std::max(o->getVelocity().magnitude() * 3, 2.0));

			bulletMan_->shoot(o, p, v);
		}
	}
}
