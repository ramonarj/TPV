#pragma once
#include "RenderComponent.h"

class GameMsgRenderer : public RenderComponent
{
public:
	GameMsgRenderer();
	~GameMsgRenderer();

	virtual void render(GameObject* o, Uint32 time);
};

