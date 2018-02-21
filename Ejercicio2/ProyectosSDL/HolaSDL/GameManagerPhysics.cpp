#include "GameManagerPhysics.h"
#include "GameManager.h"
#include "messages.h"

GameManagerPhysics::GameManagerPhysics() {
	// TODO Auto-generated constructor stub

}

GameManagerPhysics::~GameManagerPhysics() {
	// TODO Auto-generated destructor stub
}

void GameManagerPhysics::update(GameObject* o, Uint32 time) {

	GameManager* gm = static_cast<GameManager*>(o);
	GameObject* ball_ = gm->getBall();
	SDLGame* game_ = gm->getGame();

// check if the back exit from sides
	if (ball_->getPosition().getX() <= 0) {
		gm->setRightScore( gm->getRightScore()+1);
		gm->setRunning(false);
		ball_->setVelocity(Vector2D(0, 0));
		ball_->setPosition(
				Vector2D(game_->getWindowWidth() / 2 - 6,
						game_->getWindowHeight() / 2 - 6));

	} else if (ball_->getPosition().getX() - ball_->getWidth()
			>= game_->getWindowWidth()) {
		gm->setLeftScore( gm->getLeftScore()+1);
		gm->setRunning(false);
		ball_->setPosition(
				Vector2D(game_->getWindowWidth() / 2 - 6,
						game_->getWindowHeight() / 2 - 6));
		ball_->setVelocity(Vector2D(0, 0));
	}
}
