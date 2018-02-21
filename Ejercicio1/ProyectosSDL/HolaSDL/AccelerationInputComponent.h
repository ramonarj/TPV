#pragma once
#include "sdl_includes.h"
#include "GameObject.h"

double const C = 0.25;

class AccelerationInputComponent
{
public:
	AccelerationInputComponent(double thrust, double maxVel, SDL_Keycode ac, SDL_Keycode des);
	~AccelerationInputComponent();

	virtual void acceleration(GameObject* o, Uint32 time,
		const SDL_Event& event);

private:
	double thrust;
	double maxVel;
	SDL_Keycode ac;
	SDL_Keycode des;
};

