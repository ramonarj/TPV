#ifndef GAMECOMPONENT_H_
#define GAMECOMPONENT_H_

#include "GameObject.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "RotationInputComponent.h"
#include "AccelerationInputComponent.h"
#include "GunInputComponent.h"
#include <vector>

using namespace std;
/*
 *
 */
class GameComponent: public GameObject {
public:
	GameComponent(SDLGame* game);
	virtual ~GameComponent();

	virtual void handleInput(Uint32 time, const SDL_Event& event);
	virtual void update(Uint32 time);
	virtual void render(Uint32 time);
	virtual void rotation(Uint32 time, const SDL_Event& event);
	virtual void acceleration(Uint32 time, const SDL_Event& event);
	virtual void shoot(Uint32 time, const SDL_Event& event);

	virtual void addInputComponent(InputComponent* ic);
	virtual void addPhysicsComponent(PhysicsComponent* pc);
	virtual void addRenderComponent(RenderComponent* rc);
	virtual void addRotationInputComponent(RotationInputComponent* rtc);
	virtual void addAccelerationInputComponent(AccelerationInputComponent* aic);
	virtual void addGunInputComponent(GunInputComponent* gic);

	virtual void delInputComponent(InputComponent* ic);
	virtual void delPhysicsComponent(PhysicsComponent* pc);
	virtual void delRenderComponent(RenderComponent* rc);
	virtual void delRotationInputComponent(RotationInputComponent* rtc);
	virtual void delAccelerationInputComponent(AccelerationInputComponent* aic);
	virtual void delGunInputComponent(GunInputComponent* gic);

private:
	vector<InputComponent*> inputComp_;
	vector<PhysicsComponent*> physicsComp_;
	vector<RenderComponent*> renderComp_;
	vector<RotationInputComponent*> rotationComp_;
	vector<AccelerationInputComponent*> accelerationComp_;
	vector<GunInputComponent*> gunComp_;
};

#endif /* GAMECOMPONENT_H_ */
