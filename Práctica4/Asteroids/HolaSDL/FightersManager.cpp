#include "FightersManager.h"



FightersManager::FightersManager()
{
	circularMotionComp_ = nullptr;
	accelerationComp_ = nullptr;
	renderComp_ = nullptr;
	rotationComp_ = nullptr;
	gunComp1_ = nullptr;
	gunComp2_ = nullptr;
	badgeRenderer_ = nullptr;

	skeRender_ = nullptr;
}

FightersManager::FightersManager(SDLGame * game, Observer* bulletsManager)
	: GameObject(game), Observer()
{

	SDL_Rect rect
		RECT(0, 0, 415 / 8, 185 / 4);

	circularMotionComp_ = new CircularMotionPhysics();
	accelerationComp_ = new AccelerationInputComponent(0.5, 10.0, SDLK_UP, SDLK_DOWN);
	renderComp_ = new ImageRenderer(game_->getResources()->getImageTexture(Resources::SpaceShips), rect);
	rotationComp_ = new RotationInputComp(5, SDLK_RIGHT, SDLK_LEFT);
	gunComp1_ = new GunInputComponent(dynamic_cast<BulletsManager*>(bulletsManager), SDLK_SPACE, 5, 3000);
	gunComp2_ = new GunInputComponent(dynamic_cast<BulletsManager*>(bulletsManager), SDLK_SPACE, 10000, 3000);
	badgeRenderer_ = new BadgeRenderer(game_->getResources()->getImageTexture(Resources::Badges));

	skeRender_ = new SkeletonRendered();

	fighter_ = new Fighter(game, 1);
	fighter_->setWidth(50);
	fighter_->setHeight(50);

	fighter_->addPhysicsComponent(circularMotionComp_);
	fighter_->addInputComponent(accelerationComp_);
	fighter_->addRenderComponent(renderComp_);
	fighter_->addInputComponent(rotationComp_);
	fighter_->addInputComponent(gunComp1_);
	fighter_->addRenderComponent(badgeRenderer_);

	fighter_->addRenderComponent(skeRender_);
}


FightersManager::~FightersManager()
{
	if (fighter_ != nullptr)
		delete fighter_;
	if (skeRender_ != nullptr)
		delete skeRender_;
	if (badgeRenderer_ != nullptr)
		delete badgeRenderer_;
	if (gunComp2_ != nullptr)
		delete gunComp2_;
	if (gunComp1_ != nullptr)
		delete gunComp1_;
	if (rotationComp_ != nullptr)
		delete rotationComp_;
	if (renderComp_ != nullptr)
		delete renderComp_;
	if (accelerationComp_ != nullptr)
		delete accelerationComp_;
	if (circularMotionComp_ != nullptr)
		delete circularMotionComp_;
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
			fighter_->setPosition(Vector2D((double)game_->getWindowWidth() / 2 - fighter_->getWidth() / 2, (double)game_->getWindowHeight() / 2 - fighter_->getHeight() / 2));
			fighter_->setVelocity(Vector2D(0, 0));
			fighter_->setDirection(Vector2D(0, -1));
			break;
		}

		case ROUND_OVER:
		{
			fighter_->setActive(false);
			break;
		}

		case BADGE_ON:
		{
			fighter_->delInputComponent(gunComp1_);
			fighter_->addInputComponent(gunComp2_);
			fighter_->addRenderComponent(badgeRenderer_);
			break;
		}

		case BADGE_OFF:
		{
			fighter_->delInputComponent(gunComp2_);
			fighter_->addInputComponent(gunComp1_);
			fighter_->delRenderComponent(badgeRenderer_);
			break;
		}
	}
}
