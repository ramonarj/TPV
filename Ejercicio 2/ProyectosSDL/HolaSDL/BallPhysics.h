#ifndef BALLPHYSICS_H_
#define BALLPHYSICS_H_

#include "PhysicsComponent.h"
#include "Observable.h"
#include "Observer.h"

/*
 *
 */
class BallPhysics: public PhysicsComponent, public Observable, public Observer {
public:
	BallPhysics(GameObject* leftPaddle, GameObject* rightPadlle);
	virtual ~BallPhysics();
	void init(GameObject* o);
	void startRound();
	virtual void update(GameObject* o, Uint32 time);
	virtual void receive(Message msg);
private:
	GameObject* leftPaddle_;
	GameObject* rightPaddle_;
	GameObject* ball_;
};

#endif /* BALLPHYSICS_H_ */
