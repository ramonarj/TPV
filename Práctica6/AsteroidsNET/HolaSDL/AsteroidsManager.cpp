#include "AsteroidsManager.h"
#include "Collisions.h"
#include "AsteroidsGame.h"

AsteroidsManager::AsteroidsManager(SDLGame* game) : GameObject(game),
asteroidImage_(game->getResources()->getImageTexture(Resources::Astroid)), rotationPhysics_(5)
, createAsteroids_(false), createAsteroidsOn_(0), createAsteroidsTime_(10000)
{
}

AsteroidsManager::~AsteroidsManager() {
}

vector<Asteroid*>& AsteroidsManager::getAsteroids() {
	return asteroids_;
}

void AsteroidsManager::handleInput(Uint32 time, const SDL_Event& event) {
}

void AsteroidsManager::update(Uint32 time) {
	for (Asteroid* a : asteroids_)
		if (a->isActive()) {
			a->update(time);
		}

	if (createAsteroids_ && (createAsteroidsOn_ + createAsteroidsTime_ < time))
	{
		Asteroid* a = getAsteroid();

		Vector2D vel((1 - 2 * (rand() % 2))*((rand() % 10) + 1), ((rand() % 10) + 1));
		vel.normalize();
		vel = vel * 0.5;
		a->setVelocity(vel);

		Vector2D pos(rand() % getGame()->getWindowWidth(), rand() % 30);
		a->setPosition(pos);

		Vector2D dir(0, -1);
		a->setDirection(dir);

		int width = (rand() % 10) + 20;
		int height = (rand() % 10) + 20;
		a->setWidth(width);
		a->setHeight(height);

		a->setActive(true);

		createAsteroidsOn_ = SDL_GetTicks();
		AsteroidCreated m = { a->getAsteroidId(), a->getPosition(), a->getDirection(), a->getVelocity(),
		a->getWidth(), a->getHeight()};
		send(&m);
	}
}

void AsteroidsManager::render(Uint32 time) {
	for (Asteroid* a : asteroids_)
		if (a->isActive()) {
			a->render(time);
		}
}

void AsteroidsManager::receive(Message* msg) {
	switch (msg->mType_) {
		case GAME_START:
		{
			// add you code
			if (game_->isMasterClient() && !createAsteroids_)
			{
				cout << "Hola" << endl;
				createAsteroids_ = true;
				createAsteroidsOn_ = SDL_GetTicks();
			}
			break;
		}
		case GAME_OVER:
		{
			for (Asteroid* a : asteroids_)
				a->setActive(false);
			break;
		}

		case ASTEROID_CREATED:
		{
			createAsteroidMessage(msg);
		}
		//add other cases
	}
}

void AsteroidsManager::createAsteroidMessage(Message * msg)
{
	Asteroid* a = getAsteroid();
	a->setAsteroidId(static_cast<AsteroidCreated*>(msg)->asteroidId_);
	a->setPosition(static_cast<AsteroidCreated*>(msg)->pos_);
	a->setDirection(static_cast<AsteroidCreated*>(msg)->dir_);
	a->setVelocity(static_cast<AsteroidCreated*>(msg)->vel_);
	a->setWidth(static_cast<AsteroidCreated*>(msg)->width_);
	a->setHeight(static_cast<AsteroidCreated*>(msg)->height_);
	a->setActive(true);
}

Asteroid * AsteroidsManager::getAsteroid()
{
	int i = 0;
	while (i < asteroids_.size() && asteroids_.at(i)->isActive())
		i++;

	if (i == asteroids_.size())
	{
		Asteroid* a = new Asteroid(game_);
		a->addRenderComponent(&asteroidImage_);
		a->addPhysicsComponent(&destroyBorderPhysics_);
		//a->addPhysicsComponent(&circularPhysics_);
		a->addPhysicsComponent(&rotationPhysics_);
		asteroids_.push_back(a);
		return a;
	}
	else
	{
		return asteroids_.at(i);
	}
}
