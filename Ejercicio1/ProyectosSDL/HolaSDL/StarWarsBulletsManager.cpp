#include "StarWarsBulletsManager.h"


StarWarsBulletsManager::StarWarsBulletsManager(SDLGame* game) : GameObject(game), BulletsManager(), game(game)
{
}


StarWarsBulletsManager::~StarWarsBulletsManager()
{
}

void StarWarsBulletsManager::shoot(GameObject* o, Vector2D p, Vector2D v)
{
	demoComp_ = new GameComponent(game);
	fillComp_ = new FillRectRenderer({ 255, 0, 0, 255 });
	motionComp_ = new BasicMotionPhysics();

	demoComp_->setWidth(2);
	demoComp_->setHeight(2);

	bullets.push_back(demoComp_);
}

void StarWarsBulletsManager::update(Uint32 time)
{

}

void StarWarsBulletsManager::render(Uint32 time)
{

}
