#ifndef EXAMPLEGAME_H_
#define EXAMPLEGAME_H_

#include <vector>

#include "SDLGame.h"
#include "GameObject.h"
#include "StarTrekBulletManager.h"
#include "FightersManager.h"
#include "AsteroidsManager.h"
#include "CollisionManager.h"
#include "GameManager.h"
#include "SoundManager.h"


const int _WINDOW_WIDTH_ = 800;
const int _WINDOW_HEIGHT_ = 600;

class ExampleGame: public SDLGame {

public:
	ExampleGame();
	virtual ~ExampleGame();

	// from SDLGame
	void initGame();
	void closeGame();
	void start();
	void stop();

	void handleInput(Uint32 time);
	void update(Uint32 time);
	void render(Uint32 time);

private:

	bool exit_;

	vector<GameObject*> actors_;

	StarTrekBulletManager* bulletsManager_;
	FightersManager* fightersManager_;
	AsteroidsManager* asteroidsManager_;
	CollisionManager* collisionManager_;
	GameManager* gameManager_;
	SoundManager* soundManager_;
};

#endif /* EXAMPLEGAME_H_ */
