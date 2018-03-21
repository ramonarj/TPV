#pragma once
#include "RenderComponent.h"
class BadgeRenderer : public RenderComponent
{
public:
	BadgeRenderer();
	~BadgeRenderer();

	virtual void render(GameObject* o, Uint32 time);
};

