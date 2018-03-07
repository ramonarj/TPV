#include "Asteroid.h"



Asteroid::Asteroid()
{
}

Asteroid::Asteroid(SDLGame * game) : Container(game)
{
}


Asteroid::~Asteroid()
{
}

void Asteroid::setGenerations(int generations)
{
	generations_ = generations;
}

int Asteroid::getGenerations()
{
	return generations_;
}
