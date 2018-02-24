#ifndef EXAMPLEGAME_H_
#define EXAMPLEGAME_H_

#include <vector>

#include "SDLGame.h"
#include "GameObject.h"
#include "GameComponent.h"
#include "StarWarsBulletsManager.h"
#include "GunInputComponent.h"
#include "SkeletonRenderer.h"

class ExampleGame: public SDLGame {

public:
	ExampleGame();
	virtual ~ExampleGame();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame();
	void closeGame();
	void handleInput(Uint32 time);
	void update(Uint32 time);
	void render(Uint32 time);

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;
	bool exit_;
	std::vector<GameObject*> actors_;

	GameObject* demoObj_;
	GameComponent* demoComp_;
	InputComponent* inputComp_;
	PhysicsComponent* physicsComp_;
	RenderComponent* renderComp_;
	RotationInputComponent* rotationComp_;
	AccelerationInputComponent* accelerationComp_;
	GunInputComponent* gunComp_;
	SkeletonRendered* skeComp;

	GameComponent* bullet;
	StarWarsBulletsManager* bulletsMan_;
};


#endif /* EXAMPLEGAME_H_ */
