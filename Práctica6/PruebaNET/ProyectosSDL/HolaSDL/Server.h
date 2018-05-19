#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include <iostream>
#include "Messages.h"
#include "sdl_includes.h"
#include "ClientConnection.h"

using namespace std;

/*
 *
 */
class Server {
public:
	Server();
	virtual ~Server();
	void start(int port);
private:
	static int const MAX_CLIENTS = 10;
	void error();
	ClientConnection* clients[MAX_CLIENTS];
};

#endif /* SRC_SERVER_H_ */
