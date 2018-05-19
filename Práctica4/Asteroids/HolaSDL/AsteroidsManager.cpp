#include "AsteroidsManager.h"



AsteroidsManager::AsteroidsManager() : GameObject(nullptr), Observer(), Observable(), numOfAsteroids_(0)
{
	asteroidImage_ = nullptr;
	circularPhysics_ = nullptr;
	rotationPhysics_ = nullptr;

	skeRender_ = nullptr;
}

AsteroidsManager::AsteroidsManager(SDLGame * game) : GameObject(game), Observer(), Observable(), numOfAsteroids_(0)
{
	SDL_Rect rect
		RECT(0, 0, 151, 143);
	asteroidImage_ = new ImageRenderer(game->getResources()->getImageTexture(Resources::Asteroid), rect);
	circularPhysics_ = new CircularMotionPhysics();
	rotationPhysics_ = new RotationPhysics();

	skeRender_ = new SkeletonRendered({ 255, 0, 0, 255 });

	initAsteroids();
}


AsteroidsManager::~AsteroidsManager()
{
	for (int i = 0; i < asteroids_.size(); i++)
		delete asteroids_[i];

	if (skeRender_ != nullptr)
		delete skeRender_;
	if (rotationPhysics_ != nullptr)
		delete rotationPhysics_;
	if (circularPhysics_ != nullptr)
		delete circularPhysics_;
	if (asteroidImage_ != nullptr)
		delete asteroidImage_;
}

void AsteroidsManager::handleInput(Uint32 time, const SDL_Event & event)
{
}

void AsteroidsManager::update(Uint32 time)
{
	for (Asteroid* a : asteroids_)
		if (a->isActive())
			a->update(time);
}

void AsteroidsManager::render(Uint32 time)
{
	for (Asteroid* a : asteroids_)
		if (a->isActive())
			a->render(time);
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

		if (a->getGenerations() > 1)
			createBabyAsteroid(a);

		if (numOfAsteroids_ == 0)
			send(&Message(NO_MORE_ASTEROIDS));

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
		a->addRenderComponent(asteroidImage_);
		a->addPhysicsComponent(circularPhysics_);
		a->addPhysicsComponent(rotationPhysics_);
		a->addRenderComponent(skeRender_);
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
		a->setGenerations(3);

		Vector2D velocity(double(rand() % 4) - 2, double(rand() % 4) - 2);
		//velocity.set(0, 0);
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

		a->setWidth(25);
		a->setHeight(25);
	}

	numOfAsteroids_ = 5;
}

void AsteroidsManager::createBabyAsteroid(Asteroid* a)
{
	int generations = a->getGenerations();
	Vector2D velocity = a->getVelocity();
	Vector2D position = a->getPosition();
	int width = a->getWidth();
	int height = a->getHeight();
	int random = 2 + rand() % 3;
	for (int i = 0; i < random; i++)
	{
		a = getAsteroid();
		a->setActive(true);

		velocity.rotate(i * 30);

		a->setVelocity(velocity);
		a->setPosition(position);
		a->setWidth(2 * width / 3);
		a->setHeight(2 * height / 3);

		a->setGenerations(generations - 1);
  		numOfAsteroids_++;
	}
}
