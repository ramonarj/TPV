#pragma once
#include "InputComponent.h"

double const C = 0.25;

class AccelerationInputComponent : public InputComponent
{
public:
	AccelerationInputComponent();
	AccelerationInputComponent(double thrust, double maxVel, SDL_Keycode ac, SDL_Keycode des);
	~AccelerationInputComponent();

	virtual void handleInput(GameObject* o, Uint32 time, const SDL_Event& event);

private:
	double thrust;
	double maxVel;
	SDL_Keycode ac;
	SDL_Keycode des;
};

