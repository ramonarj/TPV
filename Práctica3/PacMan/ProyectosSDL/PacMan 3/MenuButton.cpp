#include "MenuButton.h"
#include "Game.h"
#include "PlayState.h"
#include <iostream>


MenuButton::MenuButton(): GameObject(nullptr) {}
MenuButton::MenuButton(Game* game, Texture* texture, SDL_Rect rect, CallBackOnClick* cbOnClick)
	: GameObject(game), texture(texture), thisRect(rect), cbOnClick(cbOnClick){}

void MenuButton:: render()
{
	texture->render(juego->getRenderer(), thisRect);
}

void MenuButton::update()
{

}

bool MenuButton::handleEvent(SDL_Event& e)
{
	SDL_Point point = { e.button.x, e.button.y };
	if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		if (SDL_PointInRect(&point, &thisRect))
		{
			cbOnClick(juego);
			return true;
		}
		else
			return false;
	}
	else
		return false;


}

MenuButton::~MenuButton(){}
