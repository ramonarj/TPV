#include "Fighter.h"



Fighter::Fighter()
{
}

Fighter::Fighter(SDLGame * game, int id): Container(game), id_(id)
{
}


Fighter::~Fighter()
{
}

int Fighter::getId()
{
	return id_;
}
