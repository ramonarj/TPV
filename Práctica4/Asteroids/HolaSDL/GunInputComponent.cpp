#include "GunInputComponent.h"



GunInputComponent::GunInputComponent()
	: bulletMan_(nullptr), shootKey(NULL), shotsPerInterval(0), timeInterval(0), numShots(0), time_(0)
{
}

GunInputComponent::GunInputComponent(BulletsManager * bulletMan_, SDL_Keycode shootKey, Uint8 shotsPerInterval, Uint32 timeInterval)
	: bulletMan_(bulletMan_), shootKey(shootKey), shotsPerInterval(shotsPerInterval), timeInterval(timeInterval), numShots(0), time_(0)
{
}


GunInputComponent::~GunInputComponent()
{
}

void GunInputComponent::handleInput(GameObject * o, Uint32 time, const SDL_Event & event)
{
	if (time_ < timeInterval)
	{
		if (numShots < shotsPerInterval) 
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == shootKey) 
				{
					bulletMan_->send(FIGHTER_SHOOT);

					numShots++;
				}
			}
		}
	}
	else
	{
		time_ = 0;
		numShots = 0;
	}

	time_++;
}
