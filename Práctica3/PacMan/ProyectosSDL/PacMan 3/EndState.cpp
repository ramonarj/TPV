#include "EndState.h"
#include "Game.h"
#include "MenuButton.h"


EndState::EndState(): GameState(){}
EndState::EndState(Game*juego, HUD*hud) : GameState(juego, hud)
{
	juego->getHud()->buttonEnd();
	//PONER LAS FUNCIONES CORRESPONDIENTES
	stage.push_back(new MenuButton(juego, juego->getTexture(BackMenuText), hud->getRect(BackMenuText), BackToMenu));
	stage.push_back(new MenuButton(juego, juego->getTexture(ExitText), hud->getRect(ExitText), ExitGame));
	hud->renderHighScores();
}

void EndState::BackToMenu(Game* juego)
{
	juego->getStateMachine()->popState();
	juego->getStateMachine()->popState();
}

void EndState::ExitGame(Game* juego)
{
	juego->exitGame();
}

void EndState::render()
{
	hud->renderHighScores();
	GameState::render();
}
