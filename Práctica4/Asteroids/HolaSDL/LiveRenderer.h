#pragma once
#include "RenderComponent.h"

class LiveRenderer : public RenderComponent
{
public:
	LiveRenderer();
	LiveRenderer(Texture* image);
	~LiveRenderer();

	virtual void render(GameObject* o, Uint32 time);

private:
	Texture* image_;
	SDL_Rect rect_;
};

