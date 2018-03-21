#pragma once
#include "InputComponent.h"
class RotationInputComp : public InputComponent
{
public:
	RotationInputComp();
	RotationInputComp(double angle, SDL_Keycode clockwise, SDL_Keycode counter_clockwise);
	~RotationInputComp();

	virtual void handleInput(GameObject* o, Uint32 time, const SDL_Event& event);

private:
	double angle;
	SDL_Keycode clockwise;
	SDL_Keycode counter_clockwise;
};

