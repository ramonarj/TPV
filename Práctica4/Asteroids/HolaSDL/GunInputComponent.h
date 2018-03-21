#pragma once
#include <algorithm>
#include "InputComponent.h"
#include "BulletsManager.h"

class GunInputComponent : public InputComponent
{
public:
	GunInputComponent();
	GunInputComponent(BulletsManager* bulletMan_, SDL_Keycode shootKey, Uint8 shotsPerInterval, Uint32 timeInterval);
	~GunInputComponent();

	virtual void handleInput(GameObject* o, Uint32 time, const SDL_Event& event);

private:
	BulletsManager* bulletMan_;

	SDL_Keycode shootKey;

	Uint8 shotsPerInterval;
	Uint8 numShots;

	Uint32 timeInterval;
	Uint32 time_;
};

