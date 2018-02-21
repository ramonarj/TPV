#ifndef EXAMPLEGAME_H_
#define EXAMPLEGAME_H_

#include <vector>

#include "SDLGame.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ComponentSwitcher.h"
#include "Container.h"
#include "SoundManager.h"

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

	Container* leftPaddle_;
	Container* rightPaddle_;
	Container* ball_;
	Container* gameManager_;

	InputComponent* keyboardIC1_;
	InputComponent* keyboardIC2_;
	InputComponent* mouseIC1_;
	InputComponent* gameCtrlIC_;

	PhysicsComponent* paddlePC_;
	PhysicsComponent* paddleAIPC_;
	PhysicsComponent* ballPC_;
	PhysicsComponent* gameManagerPC_;

	RenderComponent* fillrectRC_;
	RenderComponent* transrectRC_;
	RenderComponent* tennisballRC_;
	RenderComponent* scoreRC_;
	RenderComponent* gameMsgRC_;

	RenderComponent* keyIconRC_;
	RenderComponent* mouseIconRC_;
	RenderComponent* aiIconRC_;

	ComponentSwitcher* cs1_;
	ComponentSwitcher* cs2_;

	SoundManager soundManager_;
};

#endif /* EXAMPLEGAME_H_ */
