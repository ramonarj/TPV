#include "PauseState.h"
#include "PlayState.h"
#include "Game.h"
#include "MenuButton.h"


PauseState::PauseState(): GameState(), playState(nullptr){}
PauseState::PauseState(Game* juego, PlayState* playState, HUD* hud) : GameState(juego, hud), playState(playState)
{
	juego->getHud()->buttonPausa();
	//PONER LAS TEXTURAS Y FUNCIONES CORRESPONDIENTES
	stage.push_back(new MenuButton(juego, juego->getTexture(ReanudarText), hud->getRect(ReanudarText), BackToGame));
	stage.push_back(new MenuButton(juego, juego->getTexture(SaveText), hud->getRect(SaveText), SaveGame));
	stage.push_back(new MenuButton(juego, juego->getTexture(BackMenuText), hud->getRect(BackMenuText), BackToMenu));
}

void PauseState::render()
{
	juego->getTexture(PauseText)->render(juego->getRenderer(), hud->getRect(PauseText));
	GameState::render();
}

void PauseState::BackToGame(Game* juego)
{
	juego->getStateMachine()->popState();
}

void PauseState::SaveGame(Game* juego)
{

	juego->getStateMachine()->popState();

	PlayState* playState = dynamic_cast<PlayState*>((juego->getStateMachine()->currentState()));


	if (playState != nullptr)
	{
		juego->getHud()->changeRect(NumbersText, juego->getHud()->getRect(SaveText).x + juego->getHud()->getRect(SaveText).w,
			juego->getHud()->getRect(SaveText).y + juego->getHud()->getRect(SaveText).h / 4, (int)juego->getWindowSize(0) / 45,
			(int)juego->getWindowSize(1) / 30);
		int code = juego->enterCode();
		playState->SaveGame(code);
	}

}

void PauseState::BackToMenu(Game* juego)
{
	juego->getStateMachine()->popState();
	juego->getStateMachine()->popState();
}
