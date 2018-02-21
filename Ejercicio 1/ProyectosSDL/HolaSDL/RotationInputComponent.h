#pragma once
#include "sdl_includes.h"
#include "GameObject.h"

class RotationInputComponent
{
public:
	RotationInputComponent(double angle, SDL_Keycode clockwise, SDL_Keycode counter_clockwise);
	~RotationInputComponent();

	virtual void rotation(GameObject* o, Uint32 time,
		const SDL_Event& event);

private:
	double angle;
	SDL_Keycode clockwise;
	SDL_Keycode counter_clockwise;
};

