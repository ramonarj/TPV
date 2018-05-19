#include "GunInputComponent.h"



GunInputComponent::GunInputComponent()
	: bulletMan_(nullptr), shootKey(NULL), shotsPerInterval(0), timeInterval(0), numShots(0), time_(0)
{
}

GunInputComponent::GunInputComponent(BulletsManager * bulletMan_, SDL_Keycode shootKey, Uint8 shotsPerInterval, Uint32 timeInterval)
	: bulletMan_(bulletMan_), shootKey(shootKey), shotsPerInterval(shotsPerInterval), timeInterval(timeInterval), numShots(0)
{
	time_ = SDL_GetTicks();

	timePerShot = timeInterval / shotsPerInterval;
}


GunInputComponent::~GunInputComponent()
{
}

void GunInputComponent::handleInput(GameObject * o, Uint32 time, const SDL_Event & event)
{
	if ((time_ + timePerShot) > time)
	{
		if (numShots < shotsPerInterval) 
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == shootKey) 
				{
					Vector2D p(o->getPosition().getX() + (o->getWidth() / 2),
						o->getPosition().getY() + (o->getHeight() / 2));
					Vector2D v(o->getDirection() * std::max(o->getVelocity().magnitude() * 3, 2.0));

					FighterIsShooting a = FighterIsShooting(static_cast<Fighter*>(o), p, v);
					
					bulletMan_->send(&a);

					numShots++;
				}
			}
		}
	}
	else
	{
		time_ = SDL_GetTicks();
		numShots = 0;
	}
}
