#include "BonusManager.h"



BonusManager::BonusManager() : GameObject(nullptr), Observer(), Observable()
{
}

BonusManager::BonusManager(SDLGame * game) : GameObject(game), Observer(), Observable()
{
	SDL_Rect rect
		RECT(0, 0, 50, 50);
	bonusImage_ = ImageRenderer(game->getResources()->getImageTexture(Resources::Star), rect);

	bonus_ = new Bonus(game_);
	bonus_->setActive(false);
}


BonusManager::~BonusManager()
{
	if (bonus_ != nullptr)
		delete bonus_;
}

void BonusManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void BonusManager::update(Uint32 time)
{
	if (bonus_->isActive())
		bonus_->update(time);
}

void BonusManager::render(Uint32 time)
{
	if (bonus_->isActive())
		bonus_->render(time);
}

void BonusManager::receive(Message * msg)
{
	switch (msg->id_) {
	case BULLET_BONUS_COLLISION: {
		Fighter* f = static_cast<BulletBonusCollision*>(msg)->fighter_;
		Bullet* b = static_cast<BulletBonusCollision*>(msg)->bullet_;

		b->setActive(false);
		send(&Message(TAKE_BONUS));

		bonus_->setActive(false);
		bonus_->delRenderComponent(&bonusImage_);

		break;
	}
	case ROUND_START: {
		initBonus();
		bonus_->addRenderComponent(&bonusImage_);
		break;
	}
	case ROUND_OVER: {
		bonus_->delRenderComponent(&bonusImage_);
		break;
	}
	default:
		break;
	}
}

Bonus * BonusManager::getBonus()
{
	return bonus_;
}

void BonusManager::initBonus()
{
	bonus_->setActive(true);
	bonus_->setWidth(50);
	bonus_->setHeight(50);

	bonus_->setPosition(Vector2D(300, 300));
	bonus_->setDirection(Vector2D(0, -1));
}
