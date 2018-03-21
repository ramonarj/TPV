#include "FightersManager.h"



FightersManager::FightersManager()
{
}

FightersManager::FightersManager(SDLGame * game, Observer* bulletsManager)
	: GameObject(game), Observer()
{
	circularMotionComp_ = CircularMotionPhysics();
	accelerationComp_ = AccelerationInputComponent(0.5, 10.0, SDLK_UP, SDLK_DOWN);
	renderComp_ = ImageRenderer();
	rotationComp_ = RotationInputComp(5, SDLK_RIGHT, SDLK_LEFT);
	gunComp1_ = GunInputComponent(dynamic_cast<BulletsManager*>(bulletsManager), SDLK_SPACE, (Uint8)5, (Uint32)3);
	//dynamic_cast<Observable*>(&gunComp1_)->registerObserver(bulletsManager);
	gunComp2_ = GunInputComponent(dynamic_cast<BulletsManager*>(bulletsManager), SDLK_SPACE, (Uint8)10000, (Uint32)30);
	//dynamic_cast<Observable*>(&gunComp2_)->registerObserver(bulletsManager);
	badgeRenderer_ = BadgeRenderer();

	fighter_ = new Fighter(game, 1);

	fighter_->addPhysicsComponent(&circularMotionComp_);
	fighter_->addInputComponent(&accelerationComp_);
	fighter_->addRenderComponent(&renderComp_);
	fighter_->addInputComponent(&rotationComp_);
	fighter_->addInputComponent(&gunComp1_);

}


FightersManager::~FightersManager()
{
}

void FightersManager::handleInput(Uint32 time, const SDL_Event & event)
{
	if (fighter_->isActive())
		fighter_->handleInput(time, event);
}

void FightersManager::update(Uint32 time)
{
	if (fighter_->isActive())
		fighter_->update(time);
}

void FightersManager::render(Uint32 time)
{
	if (fighter_->isActive())
		fighter_->render(time);
}

void FightersManager::receive(Message * msg)
{
	switch (msg->id_) {
		case ROUND_START:
		{
			fighter_->setActive(true);
			fighter_->setPosition(Vector2D(game_->getWindowWidth() / 2, game_->getWindowHeight() / 2));
			break;
		}

		case ROUND_OVER:
		{
			fighter_->setActive(false);
			break;
		}

		case BADGE_ON:
		{
			fighter_->delInputComponent(&gunComp1_);
			fighter_->addInputComponent(&gunComp2_);
			fighter_->addRenderComponent(&badgeRenderer_);
			break;
		}

		case BADGE_OFF:
		{
			fighter_->delInputComponent(&gunComp2_);
			fighter_->addInputComponent(&gunComp1_);
			fighter_->delRenderComponent(&badgeRenderer_);
			break;
		}
	}
}
