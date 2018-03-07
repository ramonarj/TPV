#pragma once

#include "BulletsManager.h"
#include "sdl_includes.h"
#include "GameObject.h"

class GunInputComponent {
public:
	GunInputComponent(BulletsManager* bulletMan_, SDL_Keycode shootKey, Uint8 shotsPerInterval, Uint32 timeInterval);
	~GunInputComponent();

	virtual void GunInputComponent::handleInput(GameObject* o, Uint32 time,
		const SDL_Event& event);

private:
	BulletsManager* bulletMan_;
	SDL_Keycode shootKey;
	Uint8 shotsPerInterval;
	Uint8 numShots;
	Uint32 timeInterval;
	Uint32 time_;
};

