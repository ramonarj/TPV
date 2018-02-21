#ifndef GAMEMANAGERPHYSICS_H_
#define GAMEMANAGERPHYSICS_H_

#include "PhysicsComponent.h"

/*
 *
 */
class GameManagerPhysics: public PhysicsComponent {
public:
	GameManagerPhysics();
	virtual ~GameManagerPhysics();
	virtual void update(GameObject* o, Uint32 time);
};

#endif /* GAMEMANAGERPHYSICS_H_ */
