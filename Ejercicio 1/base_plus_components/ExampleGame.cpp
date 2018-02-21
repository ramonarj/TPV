#include "ExampleGame.h"
#include "DemoActor.h"
#include "Collisions.h"
#include "BasicKBCtrlComponent.h"
#include "BasicMotionPhysics.h"
#include "FillRectRenderer.h"
#include "ImageRenderer.h"

ExampleGame::ExampleGame() :
		SDLGame("Example Game", _WINDOW_WIDTH_, _WINDOW_HEIGHT_) {
	initGame();
	exit_ = false;
}

ExampleGame::~ExampleGame() {
	closeGame();
}

void ExampleGame::initGame() {

	// hide cursor
	SDL_ShowCursor(0);

	// normal game object
	demoObj_ = new DemoActor(this);
	demoObj_->setWidth(10);
	demoObj_->setHeight(10);
	demoObj_->setPosition(
			Vector2D(getWindowWidth() / 2 - 5, getWindowHeight() / 2 - 5));
	demoObj_->setVelocity(Vector2D(1, 1));
	actors_.push_back(demoObj_);

	// game object based on component
	demoComp_ = new GameComponent(this);
	inputComp_ = new BasicKBCtrlComponent(SDLK_a, SDLK_s, SDLK_w, SDLK_z,
			SDLK_d);
	physicsComp_ = new BasicMotionPhysics();

	// choose either the filled rectangle or the image renderer
	//
	//	renderComp_ = new FillRectRenderer( { COLOR(0x11ff22ff) });
	renderComp_ = new ImageRenderer( getResources()->getImageTexture(Resources::Star));

	demoComp_->setWidth(50);
	demoComp_->setHeight(50);
	demoComp_->setPosition(Vector2D(100, 100));
	demoComp_->setVelocity(Vector2D(1, 0));
	demoComp_->addInputComponent(inputComp_);
	demoComp_->addPhysicsComponent(physicsComp_);
	demoComp_->addRenderComponent(renderComp_);
	actors_.push_back(demoComp_);
}

void ExampleGame::closeGame() {
	if (demoObj_ != nullptr)
		delete demoObj_;
	if (demoComp_ != nullptr)
		delete demoComp_;
	if (inputComp_ != nullptr)
		delete inputComp_;
	if (physicsComp_ != nullptr)
		delete physicsComp_;
	if (renderComp_ != nullptr)
		delete renderComp_;
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

