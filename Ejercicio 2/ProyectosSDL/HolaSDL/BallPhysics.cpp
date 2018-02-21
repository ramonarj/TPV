#include "BallPhysics.h"
#include "Collisions.h"
#include "messages.h"

BallPhysics::BallPhysics(GameObject* leftPaddle,
		GameObject* rightPadlle) :
		leftPaddle_(leftPaddle), rightPaddle_(rightPadlle) {
	// TODO Auto-generated constructor stub

}

BallPhysics::~BallPhysics() {
	// TODO Auto-generated destructor stub
}

void BallPhysics::update(GameObject* o, Uint32 time) {

	Vector2D velocity = o->getVelocity();
	Vector2D position = o->getPosition()+velocity;

	double height = o->getHeight();

	// upper wall
	if ( position.getY()+height >= o->getGame()->getWindowHeight() ) {
		velocity.setY( -velocity.getY() );

		Message m { BALL_TOUCHES_WALL };
		send(m);
//		o->getGame()->getResources()->getSoundEffect(Resources::Wall_Hit)->play(0);
	}

	// lower wall
	if ( position.getY() <= 0 ) {
		velocity.setY( -velocity.getY() );
		// o->getGame()->getResources()->getSoundEffect(Resources::Wall_Hit)->play(0);
		Message m {  BALL_TOUCHES_WALL  };
		send(m);
	}

	o->setPosition(position);

	// check for collision of ball with paddles
	if (Collisions::collides(o, leftPaddle_)
			|| Collisions::collides(o, rightPaddle_)) {
		velocity.setX(-velocity.getX());
		velocity = velocity*1.2;
		// o->getGame()->getResources()->getSoundEffect(Resources::Paddle_Hit)->play(0);
		Message m { BALL_TOUCHES_PADDLE };
		send(m);
	}

	o->setVelocity(velocity);

}
