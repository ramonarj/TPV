#include <iostream>
#include <SDL2_net/SDL_net.h>

using namespace std;

static int const MAX_CLIENTS = 10;

void error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);
}

void server(int port) {
	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

	// two arrays for keeping the connections to clients, and indicating if they
	// are actually connected or the connection was closed already
	TCPsocket clients[MAX_CLIENTS];
	bool connected[MAX_CLIENTS];
	for (int i = 0; i < MAX_CLIENTS; i++)
		connected[i] = false;

	// a variable that represents the address -- in this case only the port
	IPaddress ip;

	// fill in the address address in ip -- note that the 2nd parameter is NULL
	if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) {
		error();
	}

	// Since the host in 'ip' is 0 (we provided nullptr above), SDLNet_TCP_Open starts
	// a server listening at the port specified in 'ip',  and returns a socket for listening
	// to connection requests
	TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	if (!masterSocket) {
		error();
	}

	// We want to use non blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any has some activity without blocking.
	// Non-blocking communication is the adequate one for video games!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to teh set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	while (true) {
		// the call to SDLNet_CheckSockets return the number of sockets with activity
		// in socketSet. The 0 means don't wait, if we replace it by 1000ms then it will
		// wait for at most 1000ms ...
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// accept the connection (activity on master socket is always connection
				// request sending and receiving data is done on the socket returned by
				// SDLNet_TCP_Accept. This way we can serve several clients.
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);

				// look for the first free slot in the clients array
				int j = 0;
				while (j < MAX_CLIENTS && connected[j])
					j++;

				if (j < MAX_CLIENTS) {
					cout << "Client " << j << " has connected ..." << endl;
					// we keep the socket at position j, and add it to the sockets set so we
					// can ask for activity
					clients[j] = client;
					connected[j] = true;
					SDLNet_TCP_AddSocket(socketSet, client);

					// send the client her/his ID -- no need for bzero since we are not dealing
					// with strings anymore
					buffer[0] = j;
					SDLNet_TCP_Send(client, buffer, 1); // normally we should check for errors, but I skip it

					// tell all clients that there is a new client, so they broadcast their state
					buffer[0] = -1;
					buffer[1] = j;
					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (connected[i]) {
							SDLNet_TCP_Send(clients[i], buffer, 2); // normally we should check for errors, but I skip it
						}
					}
				} else { // if there is no free slot we close the connection
					SDLNet_TCP_Close(client);
				}

			}

			// check activity from clients
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (connected[i] && SDLNet_SocketReady(clients[i])) {
					// receive a message from the i-th client, no need for bzero since we are not dealing with strings
					result = SDLNet_TCP_Recv(clients[i], buffer, 255);
					if (result > 0) {
						// if no error, forward the message to all clients, the server has no idea
						// what the message represents, etc., it treats it as a sequence of bytes
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (i != j && connected[j]) {
								SDLNet_TCP_Send(clients[j], buffer, result); // normally we should check for error
							}
						}
					} else {
						// either the client closed the socket, or something went wrong, so we close the socket
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

	// some auxiliary variables for sending/receiving information
	char buffer[256];
	int result = 0;

	// the state of all clients and this client id
	int state[MAX_CLIENTS];
	int client_id;
	for (int i = 0; i < MAX_CLIENTS; i++)
		state[i] = 0;

	// a variable that represents the address of the server we want to connect to
	IPaddress ip;

	// fill in the address in 'ip'
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {
		error();
	}

	// establish the connection with the server
	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) {
		error();
	}

	// according to the protocol, we should receive the client ID first
	result = SDLNet_TCP_Recv(conn, buffer, 1);
	if (result != 1) {
		error(); // something went wrong
	}

	// the server sends a byte with the message ID -- we know this since we have a protocol!
	client_id = buffer[0];
	cout << "Connected as client " << client_id << endl;

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
				// Get a message from the server ...
				result = SDLNet_TCP_Recv(conn, buffer, 2);
				if (result != 2) {
					error(); // something went wrong
				} else {
					int id = buffer[0];

					// according to the protocol, if the first byte is -1, then the server
					// is telling that a new client has connected and its id is in buffer[1].
					// We send a message to all clients with the current state -- we could
					// send only to the new client but this requires changing the server a
					// bit, etc., otherwise it is a message from a client broadcasting a state
					if (id == -1) {
						buffer[0] = client_id;
						buffer[1] = state[client_id];
						SDLNet_TCP_Send(conn, buffer, 2); // normally we should check for errors
					} else {
						int v = buffer[1];
						state[id] = v;
						stateChanged = true;
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
			buffer[0] = client_id;
			buffer[1] = state[client_id];
			SDLNet_TCP_Send(conn, buffer, 2); // normally we should check for errors
		}

		// if the state was changed print it
		if (stateChanged) {
			for (int i = 0; i < MAX_CLIENTS; i++) {
				cout << " " << (i == client_id ? "(" : "") << state[i]
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
