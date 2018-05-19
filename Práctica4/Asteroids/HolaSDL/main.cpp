#include <math.h>
#include "ExampleGame.h"
#include "checkML.h"

using namespace std;

int main(int ac, char** av) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ExampleGame g;
	g.start();

	return 0;
}
