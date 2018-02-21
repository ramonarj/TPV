#include "BallPhysics.h"
#include "Collisions.h"
#include "messages.h"

BallPhysics::BallPhysics(GameObject* leftPaddle,
		GameObject* rightPadlle) :
		leftPaddle_(leftPaddle), rightPaddle_(rightPadlle), ball_(nullptr) {
	// TODO Auto-generated constructor stub

}

BallPhysics::~BallPhysics() {
	// TODO Auto-generated destructor stub
}

void BallPhysics::receive(Message msg)
{
	switch (msg.id_) {
	case ROUND_START:
		startRound();
		break;
	}
}

void BallPhysics::init(GameObject* o)
{
	ball_ = o;

}

void BallPhysics::startRound()
{
	int dx = 1 - 2 * (rand() % 2); // 1 or -1
	int dy = 1 - 2 * (rand() % 2); // 1 or -1
	Vector2D v(dx * ((rand() % 5) + 2), dy * ((rand() % 5) + 2));
	v.normalize();
	ball_->setVelocity(v * 2);
}

void BallPhysics::update(GameObject* o, Uint32 time) {

	Vector2D velocity = ball_->getVelocity();
	Vector2D position = ball_->getPosition() + velocity;

	double height = ball_->getHeight();

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
