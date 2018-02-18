#include "GameStateMachine.h"


GameStateMachine::GameStateMachine(){}

void GameStateMachine::pushState(GameState* estado)
{
	states.push(estado);
}

void GameStateMachine::popState()
{
	if (!states.empty())
	{
		delete states.top();
		states.pop();
	}
}

GameState* GameStateMachine::currentState()
{
	return states.top();
}

void GameStateMachine::changeState(GameState* estado)
{
	popState();
	pushState(estado);
}


GameStateMachine::~GameStateMachine()
{
	while (!states.empty())
		popState();
}
