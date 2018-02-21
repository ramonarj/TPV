#include "GameCtrlInputComponent.h"
#include "GameManager.h"

GameCtrlInputComponent::GameCtrlInputComponent() {
}

GameCtrlInputComponent::~GameCtrlInputComponent() {
}

void GameCtrlInputComponent::handleInput(GameObject* o, Uint32 time, const SDL_Event& event) {

	GameManager* gm = static_cast<GameManager*>(o);

	if (event.type == SDL_KEYDOWN) {

		// if any key pressed while not running, we choose a random velocity of the ball
		if (!gm->isRunning()) {
			gm->setRunning(true);

			int dx = 1 - 2 * (rand() % 2); // 1 or -1
			int dy = 1 - 2 * (rand() % 2); // 1 or -1
			Vector2D v(dx * ((rand() % 5) + 2), dy * ((rand() % 5) + 2));
			v.normalize();
			gm->getBall()->setVelocity(v * 2);

			// rest the score if the game is over
			if (gm->isGameOver()) {
				gm->setLeftScore(0);
				gm->setRightScore(0);
			}
		}
	}

}
