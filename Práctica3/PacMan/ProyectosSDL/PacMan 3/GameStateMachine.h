#pragma once
#include "GameState.h"
#include <stack>

using namespace std;

class GameState;
class GameStateMachine
{
private:
	stack<GameState*> states;

public:
	//Elimina un estado de la pila
	void popState();

	//Devuelve el estado actual
	GameState* currentState();

	//Introduce un nuevo estado
	void pushState(GameState* estado);

	//Cambia de estado haciendo pop y luego push
	void changeState(GameState* estado);

	GameStateMachine();
	~GameStateMachine();
};

