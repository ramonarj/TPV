#ifndef IMAGERENDERER_H_
#define IMAGERENDERER_H_

#include "RenderComponent.h"

/*
 *
 */
class ImageRenderer: public RenderComponent {
public:
	ImageRenderer();
	ImageRenderer(Texture* image, SDL_Rect rect);
	virtual ~ImageRenderer();
	virtual void render(GameObject* o, Uint32 time);
	double calcularAngulo(GameObject* o);

private:
	Texture* image_;
	SDL_Rect r;
};

#endif /* IMAGERENDERER_H_ */
