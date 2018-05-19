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
	// We add sockets to this set and then we can ask if any has some activity without blocking.
	// Non-blocking communication is the adequate one for video games!
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1000);

	// add the masterSocket to the set
	SDLNet_TCP_AddSocket(socketSet, masterSocket);

	while (true) {
		// the call to SDLNet_CheckSockets returns the number of sockets with activity
		// in socketSet. The 2nd parameter 0 means don't wait if there is no activity,
		// if we replace it by 1000ms then it will wait up to 1000ms ...
		if (SDLNet_CheckSockets(socketSet, 0) > 0) {

			// if there is an activity in masterSocket we process it. Note that
			// before calling SDLNet_SocketReady we must have called SDLNet_CheckSockets
			if (SDLNet_SocketReady(masterSocket)) {

				// accept the connection (activity on master socket is always a connection
				// request, sending and receiving data is done via the socket returned by
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
				} else { // if there is no free slot we close the connection
					SDLNet_TCP_Close(client);
				}

			}

			// check activity from clients
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (connected[i] && SDLNet_SocketReady(clients[i])) {
					// receive a message from the i-th client, we assume it is a text of at most 255 chars
					bzero(buffer, 256);
					result = SDLNet_TCP_Recv(clients[i], buffer, 255);
					if (result > 0) {
						// if no error, print the message and send back the text 'Received!'
						cout << "Client " << i << " says: " << buffer << endl;
						SDLNet_TCP_Send(clients[i], "Received!", 9);
						// normally we should check errors afterward sending, but I skip it for simplicity
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

	while (true) {
		// ask the user for a message ...
		cout << "Enter a message: ";
		bzero(buffer, 256);
		cin.getline(buffer, 255);

		// if the user type 'exit' we stop the client, otherwise we
		// keep executing the rest of the loop body
		if (strcmp(buffer, "exit") == 0)
			break;

		// send the message to the server
		int size = strlen(buffer);
		result = SDLNet_TCP_Send(conn, buffer, size);
		if (result != size) {
			error(); // if send returns a number less than the bytes we have sent then something went wrong
		}

		// Now we wait for the server's response
		bzero(buffer, 256);
		result = SDLNet_TCP_Recv(conn, buffer, 255);
		if (result < 1) {
			error(); // something went wrong
		} else if (result == 0) {
			cout << "The server closed the connection without any message!"
					<< endl;
			break;
		} else {
			cout << "Server says: " << buffer << endl;
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
