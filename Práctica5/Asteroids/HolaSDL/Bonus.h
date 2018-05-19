#pragma once
#include "Container.h"
class Bonus : public Container
{
public:
	Bonus();
	Bonus(SDLGame* game);
	~Bonus();

	virtual void update(Uint32 time);

private:
	Uint32 timeOn_;
	Uint32 timeInterval_;
};

