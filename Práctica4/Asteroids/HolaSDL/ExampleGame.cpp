#include "ExampleGame.h"
#include "DemoActor.h"
#include "Collisions.h"
#include "BasicKBCtrlComponent.h"
#include "BasicMotionPhysics.h"
#include "FillRectRenderer.h"
#include "ImageRenderer.h"
#include "Observable.h"
#include "Observer.h"


ExampleGame::ExampleGame() :
	SDLGame("AstEroids", _WINDOW_WIDTH_, _WINDOW_HEIGHT_) {
	initGame();
	exit_ = false;
}

ExampleGame::~ExampleGame()
{
	//closeGame();
}

void ExampleGame::initGame() {
	bulletsManager_ = new StarTrekBulletManager(this);
	fightersManager_ = new FightersManager(this, bulletsManager_);
	asteroidsManager_ = new AsteroidsManager(this);
	collisionManager_ = new CollisionManager(this, bulletsManager_, asteroidsManager_, fightersManager_);
	gameManager_ = new GameManager(this);
	soundManager_ = new SoundManager(this);

	//Observables del SoundManager
	collisionManager_->registerObserver(soundManager_);
	collisionManager_->registerObserver(gameManager_);
	collisionManager_->registerObserver(bulletsManager_);
	collisionManager_->registerObserver(asteroidsManager_);

	bulletsManager_->registerObserver(soundManager_);
	bulletsManager_->registerObserver(gameManager_);
	bulletsManager_->registerObserver(asteroidsManager_);

	asteroidsManager_->registerObserver(soundManager_);
	asteroidsManager_->registerObserver(gameManager_);
	asteroidsManager_->registerObserver(bulletsManager_);

	gameManager_->registerObserver(soundManager_);
	gameManager_->registerObserver(bulletsManager_);
	gameManager_->registerObserver(asteroidsManager_);

	actors_.push_back(bulletsManager_);
	actors_.push_back(fightersManager_);
	actors_.push_back(asteroidsManager_);
	actors_.push_back(collisionManager_);
	actors_.push_back(gameManager_);
}

void ExampleGame::closeGame() {
	/*if (demoObj_ != nullptr)
		delete demoObj_;
	if (demoComp_ != nullptr)
		delete demoComp_;
	if (inputComp_ != nullptr)
		delete inputComp_;
	if (physicsComp_ != nullptr)
		delete physicsComp_;
	if (renderComp_ != nullptr)
		delete renderComp_;*/
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
		if (event.type == SDL_KEYDOWN) {
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


