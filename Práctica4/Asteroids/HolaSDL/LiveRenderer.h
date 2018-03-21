#pragma once
#include "RenderComponent.h"

class LiveRenderer : public RenderComponent
{
public:
	LiveRenderer();
	~LiveRenderer();

	virtual void render(GameObject* o, Uint32 time);
};

