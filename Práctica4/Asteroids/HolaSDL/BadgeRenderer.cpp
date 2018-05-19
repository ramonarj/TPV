#include "BadgeRenderer.h"
#include "GameManager.h"


BadgeRenderer::BadgeRenderer()
{
}

BadgeRenderer::BadgeRenderer(Texture* image) : RenderComponent(), image_(image)
{
	rect_ = RECT(600 / 3, 564 / 3, 600 / 3, 564 / 3);
}


BadgeRenderer::~BadgeRenderer()
{
}

void BadgeRenderer::render(GameObject * o, Uint32 time)
{
	SDL_Rect destRect{ 0,0,0,0 };

	destRect.w = destRect.h = o->getGame()->getWindowWidth() / 15;
	destRect.y = 0;
	destRect.x = o->getGame()->getWindowWidth() - destRect.w;

	image_->render(o->getGame()->getRenderer(), destRect, &rect_);
}
