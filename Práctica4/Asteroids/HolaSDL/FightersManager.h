#pragma once
#include "GameObject.h"
#include "Observer.h"
#include "CircularMotionPhysics.h"
#include "AccelerationInputComponent.h"
#include "ImageRenderer.h"
#include "RotationInputComp.h"
#include "GunInputComponent.h"
#include "BadgeRenderer.h"

class FightersManager : public GameObject, public Observer {
public:
	FightersManager();
	FightersManager(SDLGame* game, Observer* bulletsMamager);
	virtual ~FightersManager();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);

	Fighter* getFighter() { return fighter_; };
	virtual void receive(Message* msg);

private:
	Fighter* fighter_;
	CircularMotionPhysics circularMotionComp_;
	AccelerationInputComponent accelerationComp_;
	ImageRenderer renderComp_;
	RotationInputComp rotationComp_;
	GunInputComponent gunComp1_;
	GunInputComponent gunComp2_;
	BadgeRenderer badgeRenderer_;
};

