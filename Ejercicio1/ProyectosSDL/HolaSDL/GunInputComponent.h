#pragma once

#include "BulletsManager.h"
#include "sdl_includes.h"
#include "GameObject.h"

class GunInputComponent {
public:
	GunInputComponent(BulletsManager* bulletMan_, SDL_Keycode shootKey);
	~GunInputComponent();

	virtual void GunInputComponent::handleInput(GameObject* o, Uint32 time,
		const SDL_Event& event);

private:
	BulletsManager* bulletMan_;
	SDL_Keycode shootKey;
};

