#include "ImageRenderer.h"

ImageRenderer::ImageRenderer() : RenderComponent(), image_(nullptr)
{
}

ImageRenderer::ImageRenderer(Texture* image, SDL_Rect rect) : RenderComponent(), image_(image), r(rect) {
}

ImageRenderer::~ImageRenderer() {
}

void ImageRenderer::render(GameObject* o, Uint32 time) {
	SDL_Rect rect
		RECT(o->getPosition().getX(), o->getPosition().getY(),
			o->getWidth(), o->getHeight());

	image_->render(o->getGame()->getRenderer(), rect, calcularAngulo(o), &r);
}

double ImageRenderer::calcularAngulo(GameObject* o)
{
	Vector2D v(0, -1);
	return v.angle(o->getDirection());
}