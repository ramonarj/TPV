#include "Bonus.h"



Bonus::Bonus()
{
}

Bonus::Bonus(SDLGame * game) : Container(game)
{
}


Bonus::~Bonus()
{
}

void Bonus::update(Uint32 time)
{
	if (isActive() && (timeOn_ + timeInterval_ < time))
	{
		timeOn_ = SDL_GetTicks();
		timeInterval_ = 3000;
		setPosition(Vector2D(rand() % game_->getWindowWidth(), rand() % game_->getWindowHeight()));
	}

	direction_.rotate(3);
}
