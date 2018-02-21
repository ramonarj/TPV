#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(Texture* image) : image_(image) {
}

ImageRenderer::~ImageRenderer() {
}

void ImageRenderer::render(GameObject* o, Uint32 time) {
	SDL_Rect rect
			RECT(o->getPosition().getX(), o->getPosition().getY(),
					o->getWidth(), o->getHeight());

	image_->render(o->getGame()->getRenderer(), rect);
}
