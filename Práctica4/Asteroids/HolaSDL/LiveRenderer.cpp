#include "LiveRenderer.h"
#include "GameManager.h"


LiveRenderer::LiveRenderer() : RenderComponent(), image_(nullptr)
{
}

LiveRenderer::LiveRenderer(Texture* image) : RenderComponent(), image_(image)
{
	rect_ = RECT(2 * 600 / 3, 2 * 564 / 3, 600 / 3, 564 / 3);
}


LiveRenderer::~LiveRenderer()
{
}

void LiveRenderer::render(GameObject * o, Uint32 time)
{
	GameManager* gm = static_cast<GameManager*>(o);

	for (int i = 0; i < gm->getLives(); i++)
	{
		SDL_Rect destRect{ 0,0,0,0 };
		destRect.w = destRect.h = o->getGame()->getWindowWidth() / 15;
		destRect.y = o->getGame()->getWindowHeight() / 45;
		destRect.x  = o->getGame()->getWindowWidth() / 45 + destRect.w * i;

		image_->render(o->getGame()->getRenderer(), destRect, &rect_);

	}
}
