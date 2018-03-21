#pragma once
#include "RenderComponent.h"
class ScoreRenderer : public RenderComponent
{
public:
	ScoreRenderer();
	~ScoreRenderer();

	virtual void render(GameObject* o, Uint32 time);
};

