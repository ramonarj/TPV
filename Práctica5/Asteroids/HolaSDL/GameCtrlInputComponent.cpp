#include "GameCtrlInputComponent.h"
#include "GameManager.h"


GameCtrlInputComponent::GameCtrlInputComponent()
{
}


GameCtrlInputComponent::~GameCtrlInputComponent()
{
}

void GameCtrlInputComponent::handleInput(GameObject * o, Uint32 time, const SDL_Event & event)
{
	GameManager* gm = static_cast<GameManager*>(o);

	if (event.type == SDL_KEYDOWN) {

		// if any key pressed while not running, we choose a random velocity of the ball
		if (!gm->isRunning() && !gm->isGameOver()) {
			gm->setRunning(true);
		}
		if (gm->isGameOver())
		{
			o->getGame()->stop();
		}
	}
}
