#ifndef BALLPHYSICS_H_
#define BALLPHYSICS_H_

#include "PhysicsComponent.h"
#include "Observable.h"

/*
 *
 */
class BallPhysics: public PhysicsComponent, public Observable {
public:
	BallPhysics(GameObject* leftPaddle, GameObject* rightPadlle);
	virtual ~BallPhysics();
	virtual void update(GameObject* o, Uint32 time);
private:
	GameObject* leftPaddle_;
	GameObject* rightPaddle_;
};

#endif /* BALLPHYSICS_H_ */
