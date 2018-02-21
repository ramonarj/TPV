#ifndef SRC_MESSAGES_H_
#define SRC_MESSAGES_H_

enum MessageId {BALL_TOUCHES_WALL, BALL_TOUCHES_PADDLE, ROUND_START, ROUND_END};

struct Message {
	Message(MessageId id) : id_(id) {
	}
	MessageId id_;
};


#endif /* SRC_MESSAGES_H_ */
