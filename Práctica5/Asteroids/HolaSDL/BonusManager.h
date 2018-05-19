#pragma once

#include "GameObject.h"
#include "Observer.h"
#include "Observable.h"
#include "ImageRenderer.h"
#include "Bonus.h"

class BonusManager : public GameObject, public Observer, public Observable
{
public:
	BonusManager();
	BonusManager(SDLGame* game);
	~BonusManager();


	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

	virtual void receive(Message* msg);

	Bonus* getBonus();

	void initBonus();

private:
	ImageRenderer bonusImage_;
	Bonus* bonus_;
};

