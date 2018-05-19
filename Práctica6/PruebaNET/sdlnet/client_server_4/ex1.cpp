#include <iostream>
#include "sdl_includes.h"
#include "Messages.h"

using namespace std;

static int const MAX_CLIENTS = 10;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

char buffer_[MAX_MSG_SIZE];

bool sendMsg(TCPsocket conn, Message* msg) {

	int result = SDLNet_TCP_Send(conn, (char*) msg, msg->size_);
	if (result != msg->size_)
		return false;

	return true;
}

Message* recvMsg(TCPsocket conn) {
	int result;

	// read the header, which indicates the message size
	int bytesRead = 0;
	while (bytesRead < sizeof(header_t_)) {
		result = SDLNet_TCP_Recv(conn, buffer_ + bytesRead,
				sizeof(header_t_) - bytesRead);
		if (result < 1)
			return nullptr;
		else {
			bytesRead += result;
		}
	}

	header_t_ msgSize = *((header_t_*) buffer_);

	// read message
	while (bytesRead < msgSize) {
		result = SDLNet_TCP_Recv(conn, buffer_ + bytesRead,
				msgSize - bytesRead);
		if (result < 1)
			return nullptr;
		else {
			bytesRead += result;
		}
	}

	return (Message*) buffer_;
}

void server(int port) {

	// generic message;
	Message* msg;

	// two arrays for keeping the connections to clients, and indicating if they
	// are actually connected or the connection was closed already
	TCPsocket clients[MAX_CLIENTS];
	bool connected[MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++)
		connected[i] = false;

	// a variable that represents the address -- in this case only the port
	IPaddress ip;

	// fill in the address in 'ip' -- note that the 2nd parameter is 'nullptr'
	if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) {
		error();
	}

	// Since the host in 'ip' is 0 (we provided 'nullptr' above), SDLNet_TCP_Open
	// starts a server listening at the port specified in 'ip' and returns a
	// socket for listening to connection requests ...
	TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	if (!masterSocket) {
		error();
	}

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without blocking.
	// Non-blocking communication is the adequate one for video games!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to the set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	while (true) {
		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter 0 means don't wait if there is no activity.
		// If we replace it by 1000ms then it will wait up to 1000ms ...
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// Accept the connection (activity on the master socket is always a
				// connection request, sending and receiving data is done on the socket
				// returned by SDLNet_TCP_Accept. This way we can serve several
				// clients.
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);

				// look for the first free slot in the clients array
				Uint8 j = 0;
				while (j < MAX_CLIENTS && connected[j])
					j++;

				if (j < MAX_CLIENTS) {
					cout << "Client " << static_cast<int>(j)
							<< " has connected ..." << endl;

					// we keep the socket at position j, and add it to the sockets set
					// so we can ask for activity
					clients[j] = client;
					connected[j] = true;
					SDLNet_TCP_AddSocket(socketSet, client);

					// send the client her/his ID
					ConnectedMsg msg_1 { j };
					sendMsg(client, &msg_1);

					// tell all clients that there is a new client, so they broadcast
					// their state
					NewClientMsg msg_2 { j };
					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (connected[i]) {
							sendMsg(clients[i], &msg_2); // normally we should check for errors, but I skip it
						}
					}
				} else { // if there is no free slot we close the connection
					SDLNet_TCP_Close(client);
				}

			}

			// check activity from clients
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (connected[i] && SDLNet_SocketReady(clients[i])) {
					// receive a message from the i-th client,
					msg = recvMsg(clients[i]);
					if (msg != nullptr) {
						// if no error, forward the message to all clients
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (i != j && connected[j]) {
								sendMsg(clients[j], msg); // normally we should check for error
							}
						}
					} else {
						// either the client closed the socket, or something went wrong
						SDLNet_TCP_Close(clients[i]);
						connected[i] = false;
						SDLNet_TCP_DelSocket(socketSet, clients[i]); // remove it from set -- very important!!
						cout << "Client " << i << " disconnected" << endl;
					}
				}
			}
		}

		// Do something else, this is why we want non-blocking! We want to keep doing other
		// if no data is received -- for now we don't do anything actually!
	}

	// close the master socket when we exit the loop
	SDLNet_TCP_Close(masterSocket);
}

void client(char* host, int port) {

	// pointer to a generic message
	Message* msg = nullptr;

	// the state and the client id
	Uint8 state[MAX_CLIENTS];
	Uint8 client_id;
	for (int i = 0; i < MAX_CLIENTS; i++)
		state[i] = 0;

	// a variable that represents the address of the server we want to connect to
	IPaddress ip;

	// fill in the address address in 'ip'
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {
		error();
	}

	// establish the connection with the server
	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) {
		error();
	}

	// according to the protocol, we should receive the client ID first
	msg = recvMsg(conn);
	if (msg == nullptr || msg->id_ != CONNECTED) {
		error(); // something went wrong
	}

	client_id = static_cast<ConnectedMsg*>(msg)->clientId_;
	cout << "Connected as client " << static_cast<int>(client_id) << endl;

	// since we want non-blocking communication we need to use a socket set,
	// though with a single socket ...
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet, conn);

	// a boolean to indicate when the state changes, just for not printing
	// the state all the time, but rather only when it changes
	bool stateChanged = true;

	while (true) {
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {
			if (SDLNet_SocketReady(conn)) {
				// Get a message from the server
				msg = recvMsg(conn);
				if (msg == nullptr) {
					error(); // something went wrong
				} else {

					// react depending on the type of the message
					switch (msg->id_) {
					case NEW_CLIENT: { // new client connected
						StateChangedMsg msg_1 = { client_id, state[client_id] };
						sendMsg(conn, &msg_1); // normally we should check for errors
						break;
					}
					case STATE_CHANGED: { // state of some client has changed
						StateChangedMsg* msg_1 =
								static_cast<StateChangedMsg*>(msg);
						state[msg_1->clientId_] = msg_1->state_;
						stateChanged = true;
						break;
					}
					default:
						cout << "Unknown message .. "
								<< static_cast<int>(msg->id_) << endl;
					}
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
			sendMsg(conn, &msg_1); // normally we should check for errors
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
	SDLNet_TCP_Close(conn);
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
