#include "MainMenuState.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "PlayCont.h"
#include "HUD.h"
#include <iostream>


MainMenuState::MainMenuState():GameState(){}
MainMenuState::MainMenuState(Game*juego, HUD* hud) : GameState(juego, hud)
{
	juego->getHud()->buttonMenu();
	stage.push_back(new MenuButton(juego, juego->getTexture(NewGameText), hud->getRect(NewGameText), playGame));
	stage.push_back(new MenuButton(juego, juego->getTexture(LoadGameText), hud->getRect(LoadGameText), loadGame));
	stage.push_back(new MenuButton(juego, juego->getTexture(ExitText), hud->getRect(ExitText), exitGame));
}

void MainMenuState::render()
{
	juego->getTexture(MenuText)->render(juego->getRenderer(), hud->getRect(MenuText));
	GameState::render();
}

void MainMenuState::playGame(Game* juego)
{
	juego->getStateMachine()->pushState(new PlayCont(juego, juego->getHud(), "level01.pac", 10000));
}


void MainMenuState::loadGame(Game* juego)
{
	juego->getHud()->changeRect(NumbersText, juego->getHud()->getRect(LoadGameText).x + juego->getHud()->getRect(SaveText).w,
		juego->getHud()->getRect(LoadGameText).y + juego->getHud()->getRect(SaveText).h / 4, (int)juego->getWindowSize(0) / 45,
		(int)juego->getWindowSize(1) / 30);
	int code = juego->enterCode();
	string level = to_string(code) + ".pac";
	juego->getStateMachine()->pushState(new PlayState(juego, juego->getHud(), level));
}

void MainMenuState::exitGame(Game* juego)
{
	juego->exitGame();
}
