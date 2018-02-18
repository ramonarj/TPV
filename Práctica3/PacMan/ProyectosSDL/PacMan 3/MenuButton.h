#pragma once
#include "GameObject.h"
#include "Texture.h"

typedef void CallBackOnClick(Game* juego);
class MenuButton : public GameObject
{
protected:
	Texture* texture;
	SDL_Rect thisRect;
	CallBackOnClick* cbOnClick;

public:

	virtual void render();
	virtual void update();
	virtual bool handleEvent(SDL_Event& e);
	MenuButton();
	MenuButton(Game* game, Texture* texture, SDL_Rect rect, CallBackOnClick* cbOnClick);
	~MenuButton();
};

