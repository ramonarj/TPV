#include <iostream>
#include "sdl_includes.h"
#include "Messages.h"
#include "ClientConnection.h"
#include "Server.h"

using namespace std;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}


void server(int port) {
	Server server;
	server.start(port);
}

void client(char* host, int port) {
	static const int MAX_CLIENTS = 10;

	// pointer to a generic message
	Message* msg = nullptr;

	// the state of all clients, and the id of this client
	Uint8 state[MAX_CLIENTS];
	Uint8 client_id;
	for (int i = 0; i < MAX_CLIENTS; i++)
		state[i] = 0;

	// establish the connection with the server
	ClientConnection conn;
	if (!conn.connect(host, port)) {
		error();
	}

	// according to the protocol, we should receive the client ID first
	msg = conn.recvMessage();
	if (msg == nullptr || msg->id_ != CONNECTED) {
		error(); // something went wrong
	}

	client_id = static_cast<ConnectedMsg*>(msg)->clientId_;
	cout << "Connected as client " << static_cast<int>(client_id) << endl;

	// since we want non-blocking communication we need to use a socket set,
	// though with a single socket ...
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet, conn.getSocket());

	// a boolean to indicate when the state changes, just for not printing
	// the state all the time, but rather only when it changes ...
	bool stateChanged = true;

	while (true) {
		if (SDLNet_CheckSockets(socketSet, 0) > 0 && conn.ready()) {

			// Get a message from the server
			msg = conn.recvMessage();
			if (msg == nullptr) {
				error(); // something went wrong
			} else {

				// react depending on the type of the message ...
				switch (msg->id_) {
				case NEW_CLIENT: { // new client connected, should send my state
					StateChangedMsg msg_1 = { client_id, state[client_id] };
					conn.sendMessage(&msg_1); // normally we should check for errors
					break;
				}
				case STATE_CHANGED: { // the state of some client has changed
					StateChangedMsg* msg_1 = static_cast<StateChangedMsg*>(msg);
					state[msg_1->clientId_] = msg_1->state_;
					stateChanged = true;
					break;
				}
				default:
					cout << "Unknown message .. " << static_cast<int>(msg->id_)
							<< endl;
				}
			}
		}

		// randomly decide if to change the state of this client
		if ((rand() % 1000000) == 0) {

			// change the state to a random number between 0 and 9
			state[client_id] = rand() % 9;
			stateChanged = true;

			// send a message telling other clients of this state change
			StateChangedMsg msg_1 = { client_id, state[client_id] };
			conn.sendMessage(&msg_1); // normally we should check for errors
		}

		// if the state has changed print it
		if (stateChanged) {
			for (int i = 0; i < MAX_CLIENTS; i++) {
				cout << " " << (i == client_id ? "(" : "")
						<< static_cast<int>(state[i])
						<< (i == client_id ? ")" : "");
			}
			cout << endl;
			stateChanged = false;
		}
	}

	// close the connection
	conn.close();
}

int main(int argc, char** argv) {

// initialize SDLNet
	if (SDLNet_Init() < 0) {
		error();
	}

	if (argc == 3 && strcmp(argv[1], "server") == 0) {
		server(atoi(argv[2])); // start in server mode
	} else if (argc == 4 && strcmp(argv[1], "client") == 0) {
		client(argv[2], atoi(argv[3])); // start in client mode
	} else {
		cout << "Usage: " << endl;
		cout << "  " << argv[0] << " client host port " << endl;
		cout << "  " << argv[0] << " server port " << endl;
		cout << endl;
		cout << "Example:" << endl;
		cout << "  " << argv[0] << " server 2000" << endl;
		cout << "  " << argv[0] << " client localhost 2000" << endl;
	}

// finalize SDLNet
	SDLNet_Quit();

	return 0;
}
