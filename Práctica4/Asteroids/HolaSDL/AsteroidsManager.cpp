#include "AsteroidsManager.h"



AsteroidsManager::AsteroidsManager() : GameObject(nullptr), Observer(), Observable(), numOfAsteroids_(0)
{
}

AsteroidsManager::AsteroidsManager(SDLGame * game) : GameObject(game), Observer(), Observable(), numOfAsteroids_(0)
{
	initAsteroids();
	asteroidImage_ = ImageRenderer(game->getResources()->getImageTexture(Resources::Asteroid));
	circularPhysics_ = CircularMotionPhysics();
	rotationPhysics_ = RotationPhysics();
}


AsteroidsManager::~AsteroidsManager()
{
}

void AsteroidsManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void AsteroidsManager::update(Uint32 time)
{
	for (Asteroid* a : asteroids_)
		if (a->isActive()) a->update(time);
}

void AsteroidsManager::render(Uint32 time)
{
	for (Asteroid* a : asteroids_)
		if (a->isActive()) a->render(time);
}

vector<Asteroid*>& AsteroidsManager::getAsteroids()
{
	return asteroids_;
}

void AsteroidsManager::receive(Message * msg)
{
	switch (msg->id_) {
	case BULLET_ASTEROID_COLLISION: {
		Asteroid* a = static_cast<BulletAsteroidCollision*>(msg)->asteroid_;
		a->setActive(false);
		numOfAsteroids_--;
		if (a->getGenerations() > 0)
			createBabyAsteroid(a);
		else if (numOfAsteroids_ == 0)
			send(NO_MORE_ASTEROIDS);
		break;
	}
	case ROUND_START: {
		initAsteroids();
		break;
	}
	default:
		break;
	}
}



Asteroid* AsteroidsManager::getAsteroid()
{
	int i = 0;
	while (i < asteroids_.size() && asteroids_.at(i)->isActive())
		i++;

	if (i == asteroids_.size())
	{
		Asteroid* a = new Asteroid(game_);
		a->addRenderComponent(&asteroidImage_);
		a->addPhysicsComponent(&circularPhysics_);
		a->addPhysicsComponent(&rotationPhysics_);
		asteroids_.push_back(a);
		return a;
	}
	else
	{
		return asteroids_.at(i);
	}
}

void AsteroidsManager::initAsteroids()
{
	for (Asteroid* a : asteroids_)
		a->setActive(false);

	for (int i = 0; i < 5; i++)
	{
		Asteroid* a = getAsteroid();
		a->setActive(true);

		Vector2D velocity(double((rand() % RAND_MAX) / RAND_MAX), double((rand() % RAND_MAX) / RAND_MAX));
		a->setVelocity(velocity);

		Vector2D position(0, 0);
		int wall = rand() % 4;
		if (wall < 2)
		{
			position.setX(rand() % game_->getWindowWidth());
			position.setY(wall * game_->getWindowHeight());
		}
		else
		{
			position.setX((wall - 2) * game_->getWindowWidth());
			position.setY(rand() % game_->getWindowHeight());
		}
		a->setPosition(position);
	}

	numOfAsteroids_ = 5;
}

void AsteroidsManager::createBabyAsteroid(Asteroid* a)
{
	int generations = a->getGenerations();
	Vector2D velocity = a->getVelocity();
	Vector2D position = a->getPosition();
	int random = 2 + rand() % 3;
	for (int i = 0; i < random; i++)
	{
		a = getAsteroid();
		int velX = velocity.getX() * 0.8 + rand() % int(velocity.getX() * 0.4);
		int velY = sqrt(pow(velocity.magnitude(), 2) - pow(velX, 2));
		velocity.set(velX, velY);
		a->setVelocity(velocity);
		a->setPosition(position);
		a->setGenerations(generations - 1);
		numOfAsteroids_++;
	}
}
