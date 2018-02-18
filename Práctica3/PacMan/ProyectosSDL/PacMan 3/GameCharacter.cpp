#include "GameCharacter.h"
#include "Game.h"
#include <iostream>
#include "Exceptions.h"
#include "PlayState.h"

//Constructoras
GameCharacter::GameCharacter()
	:PacManObject(), texture(nullptr), posX(0), posY(0), iniX(0), iniY(0), dirX(0), dirY(0), frameRow(0), frameCol(0), sizeX(0), sizeY(0){}
GameCharacter::GameCharacter(Game*juego, Texture* texture, PlayState* playState, unsigned int frameRow, unsigned int frameCol)
	:PacManObject(juego, playState), texture(texture), posX(0), posY(0), iniX(0), iniY(0), dirX(1), dirY(0),
	frameRow(frameRow), frameCol(frameCol), sizeX(playState->getCellSize(0)), sizeY(playState->getCellSize(1)){}
GameCharacter::GameCharacter(Game*juego, Texture* texture, PlayState* playState,unsigned int frameRow, unsigned int frameCol, unsigned int posX, unsigned int posY)
	:PacManObject(juego, playState), texture(texture), posX(posX), posY(posY), iniX(posX), iniY(posY), dirX(1), dirY(0),
	frameRow(frameRow), frameCol(frameCol), sizeX(playState->getCellSize(0)), sizeY(playState->getCellSize(1)){}


void GameCharacter::render()
{
	//Creamos el rectángulo destino
	SDL_Rect destRect = { posX * playState->getCellSize(0), posY * playState->getCellSize(1),
		sizeX, sizeY };

	//Para que se pinte en medio de la celda
	destRect.x += ((playState->getCellSize(0) - sizeX) / 2);
	destRect.y += ((playState->getCellSize(1) - sizeY) / 2);

	texture->renderFrame(juego->getRenderer(), destRect, frameRow, frameCol);
}

void GameCharacter::loadFromFile(ifstream& file)
{
	file >> posY >> posX >> iniY >> iniX >> dirX >> dirY;
	if (posX > playState->getSize(1) || posX < 0 || posY > playState->getSize(0) || posY < 0
		|| iniX > playState->getSize(1) || iniX < 0 || iniY > playState->getSize(0) || iniY < 0)
		throw FileFormatError("Invalid character position");
}

void GameCharacter::saveToFile(ofstream& file)
{
	file << posY << " " << posX << " " << iniY << " " << iniX << " " << dirX << " " << dirY << " ";
}

GameCharacter::~GameCharacter()
{

}
