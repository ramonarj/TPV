#pragma once
#include "InputComponent.h"

class GameCtrlInputComponent : public InputComponent
{
public:
	GameCtrlInputComponent();
	~GameCtrlInputComponent();

	virtual void handleInput(GameObject* o, Uint32 time, const SDL_Event& event);
};

