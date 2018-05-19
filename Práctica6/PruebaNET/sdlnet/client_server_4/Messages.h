#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "sdl_includes.h"

typedef Uint16 header_t_;

enum MessageId {
	CONNECTED,
	NEW_CLIENT,
	STATE_CHANGED
};

struct Message {
	Message(MessageId id, header_t_ size = sizeof(Message)) :
			size_(size), id_(id) {
	}
	header_t_ size_;
	Uint8 id_; // we have at most 256 different kinds of messages
};

struct ConnectedMsg : Message {
	ConnectedMsg(Uint8 clientId) :
			Message(CONNECTED, sizeof(ConnectedMsg)), clientId_(clientId) {
	}
	Uint8 clientId_;
};

struct NewClientMsg : Message {
	NewClientMsg(Uint8 clientId) :
			Message(NEW_CLIENT, sizeof(NewClientMsg)), clientId_(clientId) {
	}
	Uint8 clientId_;
};

struct StateChangedMsg : Message {
	StateChangedMsg(Uint8 clientId, Uint8 state) :
			Message(STATE_CHANGED, sizeof(StateChangedMsg)), clientId_(clientId),  state_(state) {
	}
	Uint8 clientId_;
	Uint8 state_;
};

#define MAX_MSG_SIZE sizeof(StateChangedMsg)


#endif /* MESSAGES_H_ */
