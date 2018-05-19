#include "AsteroidsManager.h"

#include "Collisions.h"

AsteroidsManager::AsteroidsManager(SDLGame* game) :
		GameObject(game) {
}

AsteroidsManager::~AsteroidsManager() {
}

vector<Asteroid*>& AsteroidsManager::getAsteroids() {
	return asteroids_;
}

void AsteroidsManager::handleInput(Uint32 time, const SDL_Event& event) {
}

void AsteroidsManager::update(Uint32 time) {
	for (Asteroid* a : asteroids_)
		if (a->isActive()) {
			a->update(time);
		}
}

void AsteroidsManager::render(Uint32 time) {
	for (Asteroid* a : asteroids_)
		if (a->isActive()) {
			a->render(time);
		}
}

void AsteroidsManager::receive(Message* msg) {
	switch (msg->mType_) {
	case GAME_START:
		// add you code
		break;
		//add other cases
	}
}

