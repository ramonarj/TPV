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

	demoComp_->addRenderComponent(fillComp_);
	demoComp_->addPhysicsComponent(motionComp_);

	demoComp_->setPosition(p);
	demoComp_->setVelocity(v);
	demoComp_->setWidth(2);
	demoComp_->setHeight(2);

	bullets.push_back(demoComp_);
}

void StarWarsBulletsManager::update(Uint32 time)
{
	int i = 0;
	for (GameObject* o : bullets) {
		o->update(time);
		if (o->getPosition().getX() < 0 || o->getPosition().getX() >= game->getWindowWidth()
			|| o->getPosition().getY() < 0 || o->getPosition().getY() >= game->getWindowHeight())
		{
			/*delete o;
			bullets.erase(bullets.begin() + i);*/
		}
		i++;
	}
}

void StarWarsBulletsManager::render(Uint32 time)
{
	for (GameObject* o : bullets) {
		o->render(time);
	}
}
