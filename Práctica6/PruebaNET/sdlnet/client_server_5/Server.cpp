#include "Server.h"

Server::Server() {
	// TODO Auto-generated constructor stub

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

void Server::start(int port) {
	// initialize the connections array
	for (int i = 0; i < MAX_CLIENTS; i++)
		clients[i] = nullptr;

	// a variable that represents the address -- in this case only the port
	IPaddress ip;

	// fill in the address in 'ip' -- note that the 2nd parameter is 'nullptr'
	if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) {
		error();
	}

	// Since the host in 'ip' is 0 (we provided 'nullptr' above), SDLNet_TCP_Open starts
	// a server listening at the port specified in 'ip' and returns a socket for listening
	// to connection requests
	TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	if (!masterSocket) {
		error();
	}

	// We want to use non-blocking communication, the way to do this is via a socket set.
	// We add sockets to this set and then we can ask if any socket in this has some
	// activity without blocking.
	// Non-blocking communication is the adequate one for video games!!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to the set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	while (true) {
		// The call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter 0 tell the method no to wait if there is no
		// activity (replacing it by 1000ms then it will wait up to 1000ms)
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// accept the connection (activity on master socket is always a connection
				// request, sending and receiving data is done via the socket returned by
				// SDLNet_TCP_Accept. This way we can serve several clients ...
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);

				// look for the first free slot in the clients array
				Uint8 j = 0;
				while (j < MAX_CLIENTS && clients[j] != nullptr)
					j++;

				if (j < MAX_CLIENTS) {
					cout << "Client " << static_cast<int>(j)
							<< " has connected ..." << endl;

					// We keep the connection at position j, and add the socket to the
					// sockets set so we can ask for activity later ...
					clients[j] = new ClientConnection(client);
					SDLNet_TCP_AddSocket(socketSet, client);

					// send the client her/his ID
					ConnectedMsg msg_1 { j };
					clients[j]->sendMessage(&msg_1);

					// tell all clients that there is a new client, so they broadcast their state
					NewClientMsg msg_2 { j };
					for (int i = 0; i < MAX_CLIENTS; i++) {
						if (clients[i] != nullptr) {
							clients[i]->sendMessage(&msg_2); // normally we should check for errors, but I skip it
						}
					}
				} else { // if there is no free slot we close the connection
					SDLNet_TCP_Close(client);
				}

			}

			// check activity from clients
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i] != nullptr && clients[i]->ready()) {
					// receive a message from the i-th client
					Message *msg = clients[i]->recvMessage();
					if (msg != nullptr) {
						// if no error, forward the message to all clients
						for (int j = 0; j < MAX_CLIENTS; j++) {
							if (i != j && clients[j] != nullptr) {
								clients[j]->sendMessage(msg); // normally we should check for error
							}
						}
					} else {
						// either the client closed the connection, or something went wrong
						SDLNet_TCP_DelSocket(socketSet,
								clients[i]->getSocket()); // remove it from set -- very important!!
						clients[i]->close();
						delete clients[i];
						clients[i] = nullptr;
						cout << "Client " << i << " disconnected" << endl;
					}
				}
			}
		}

		// Do something else, this is why we want non-blocking! We want to keep doing other
		// things if no message is received -- for now we don't do anything actually!
	}

	// close the master socket when we exit the loop
	SDLNet_TCP_Close(masterSocket);
}

void Server::error() {
	cout << "Error: " << SDLNet_GetError() << endl;
	exit(1);

}
