#include "ExampleGame.h"

#include "BallPhysics.h"
#include "DemoActor.h"
#include "Collisions.h"
#include "PaddleKeyboardComponent.h"
#include "PaddlePhysicsComponent.h"
#include "FillRectRenderer.h"
#include "PaddleMouseInputComponent.h"
#include "PaddleAIPhysics.h"
#include "TranRectRenderer.h"
#include "ImageRenderer.h"
#include "GameManager.h"
#include "ScoreRenderer.h"
#include "GameMsgRenderer.h"
#include "GameCtrlInputComponent.h"
#include "GameManagerPhysics.h"

ExampleGame::ExampleGame() :
		SDLGame("Example Game", _WINDOW_WIDTH_, _WINDOW_HEIGHT_),
		soundManager_(this)
		{
	initGame();
	exit_ = false;
}

ExampleGame::~ExampleGame() {
	closeGame();
}

void ExampleGame::initGame() {

	// hide cursor
	SDL_ShowCursor(0);

	ball_ = new Container(this);
	ball_->setWidth(11);
	ball_->setHeight(11);
	ball_->setPosition(
			Vector2D(getWindowWidth() / 2 - 6, getWindowHeight() / 2 - 6));

	leftPaddle_ = new Container(this);
	leftPaddle_->setWidth(10);
	leftPaddle_->setHeight(50);
	leftPaddle_->setPosition(Vector2D(5, getWindowHeight() / 2 - 25));

	rightPaddle_ = new Container(this);
	rightPaddle_->setWidth(10);
	rightPaddle_->setHeight(50);
	rightPaddle_->setPosition(
			Vector2D(getWindowWidth() - 15, getWindowHeight() / 2 - 25));

	gameManager_ = new GameManager(this,ball_,2);

	keyboardIC1_ = new PaddleKeyboardComponent(SDLK_UP, SDLK_DOWN, SDLK_SPACE);
	keyboardIC2_ = new PaddleKeyboardComponent(SDLK_a, SDLK_z, SDLK_s);
	mouseIC1_ = new PaddleMouseInputComponent();
	gameCtrlIC_ = new GameCtrlInputComponent();

	paddlePC_ = new PaddlePhysicsComponent();
	paddleAIPC_ = new PaddleAIPhysics(ball_);
	ballPC_ = new BallPhysics(leftPaddle_, rightPaddle_);
	gameManagerPC_ = new GameManagerPhysics();

	fillrectRC_ = new FillRectRenderer();
	transrectRC_ = new TranRectRenderer();
	tennisballRC_ = new ImageRenderer(
			getResources()->getImageTexture(Resources::TennisBall));
	keyIconRC_ = new ImageRenderer(
			getResources()->getImageTexture(Resources::KeyBoardIcon));
	mouseIconRC_ = new ImageRenderer(
			getResources()->getImageTexture(Resources::MouseIcon));
	aiIconRC_ = new ImageRenderer(
			getResources()->getImageTexture(Resources::AIIcon));

	scoreRC_ = new ScoreRenderer();
	gameMsgRC_ = new GameMsgRenderer();


	cs1_ = new ComponentSwitcher(this, leftPaddle_, SDLK_1);
	cs1_->setPosition(Vector2D(5, 5));
	cs1_->setWidth(50);
	cs1_->setHeight(50);

	cs2_ = new ComponentSwitcher(this, rightPaddle_, SDLK_2);
	cs2_->setPosition(Vector2D(getWindowWidth() - 55, 5));
	cs2_->setWidth(50);
	cs2_->setHeight(50);

	cs1_->addMode( { keyboardIC1_, paddlePC_, fillrectRC_, keyIconRC_ });
	cs1_->addMode( { mouseIC1_, paddlePC_, fillrectRC_, mouseIconRC_ });
	cs1_->addMode( { nullptr, paddleAIPC_, transrectRC_, aiIconRC_ });

	cs2_->addMode( { keyboardIC2_, paddlePC_, fillrectRC_, keyIconRC_ });
	cs2_->addMode( { mouseIC1_, paddlePC_, fillrectRC_, mouseIconRC_ });
	cs2_->addMode( { nullptr, paddleAIPC_, transrectRC_, aiIconRC_ });

	ball_->addPhysicsComponent(ballPC_);
	ball_->addRenderComponent(tennisballRC_);

	gameManager_->addInputComponent(gameCtrlIC_);
	gameManager_->addRenderComponent(gameMsgRC_);
	gameManager_->addRenderComponent(scoreRC_);
	gameManager_->addPhysicsComponent(gameManagerPC_);

	cs1_->setMode(0);
	cs2_->setMode(0);

	dynamic_cast<Observable*>(ballPC_)->registerObserver(&soundManager_);
	dynamic_cast<Observable*>(gameManager_)->registerObserver(&soundManager_);

	actors_.push_back(gameManager_);
	actors_.push_back(cs1_);
	actors_.push_back(cs2_);
	actors_.push_back(leftPaddle_);
	actors_.push_back(rightPaddle_);
	actors_.push_back(ball_);
}

void ExampleGame::closeGame() {
	if (ball_ != nullptr)
		delete ball_;
	if (leftPaddle_ != nullptr)
		delete leftPaddle_;
	if (rightPaddle_ != nullptr)
		delete rightPaddle_;
	// should free all memory here, I skip it, you shoudl always do it!
}

void ExampleGame::start() {
	exit_ = false;
	while (!exit_) {
		Uint32 startTime = SDL_GetTicks();
		handleInput(startTime);
		update(startTime);

		render(startTime);
		Uint32 frameTime = SDL_GetTicks() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

void ExampleGame::stop() {
	exit_ = true;
}

void ExampleGame::handleInput(Uint32 time) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			exit_ = true;
			break;
			// Pressing f to toggle fullscreen.
		case SDLK_f:
			int flags = SDL_GetWindowFlags(window_);
			if (flags & SDL_WINDOW_FULLSCREEN) {
				SDL_SetWindowFullscreen(window_, 0);
			} else {
				SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
			}
			break;
		}
		for (GameObject* o : actors_) {
			o->handleInput(time, event);
		}
	}
}

void ExampleGame::update(Uint32 time) {
	for (GameObject* o : actors_) {
		o->update(time);
	}
}

void ExampleGame::render(Uint32 time) {
	SDL_SetRenderDrawColor(getRenderer(), COLOR(0x00AAAAFF));
	SDL_RenderClear(getRenderer());

	for (GameObject* o : actors_) {
		o->render(time);
	}

	SDL_RenderPresent(getRenderer());
}

