#include "SoundManager.h"

SoundManager::SoundManager() : Observer(), game_(nullptr)
{
}

SoundManager::SoundManager(SDLGame * game) : Observer(), game_(game)
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::receive(Message * msg)
{
	switch ( msg->id_ ) {
		case BULLET_CREATED:
		{
			game_->getResources()->getSoundEffect(Resources::GunShot)->play();
			break;
		}

		case BULLET_ASTEROID_COLLISION:
		{
			game_->getResources()->getSoundEffect(Resources::ExplosionSound)->play();
			break;
		}

		case ASTEROID_FIGHTER_COLLISION:
		{
			game_->getResources()->getSoundEffect(Resources::Paddle_Hit)->play();
			break;
		}

		case ROUND_START:
		{
			game_->getResources()->getMusic(Resources::ImperialMarch)->play();
			break;
		}

		case ROUND_OVER:
		{
			game_->getResources()->getMusic(Resources::ImperialMarch)->pause();
			break;
		}
	}
}
