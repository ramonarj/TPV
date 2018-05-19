#pragma once
#include "Observer.h"
class SoundManager : public Observer
{
public:
	SoundManager();
	SoundManager(SDLGame* game);
	virtual ~SoundManager();

	virtual void receive(Message* msg);

private:
	SDLGame* game_;
};

