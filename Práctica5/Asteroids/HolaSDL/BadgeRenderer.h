#pragma once
#include "RenderComponent.h"
class BadgeRenderer : public RenderComponent
{
public:
	BadgeRenderer();
	BadgeRenderer(Texture* image, SDL_Rect rect);
	~BadgeRenderer();

	virtual void render(GameObject* o, Uint32 time);

private:
	Texture* image_;
	SDL_Rect rect_;
};

