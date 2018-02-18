#include "GameCharacter.h"
#include "Game.h"
#include <iostream>

//Constructoras
GameCharacter::GameCharacter()
	:GameObject(nullptr), texture(nullptr), posX(0), posY(0), iniX(0), iniY(0), dirX(0), dirY(0), frameRow(0), frameCol(0), sizeX(0), sizeY(0){}
GameCharacter::GameCharacter(Game*juego, Texture* texture, unsigned int frameRow, unsigned int frameCol)
	: GameObject(juego), texture(texture), posX(0), posY(0), iniX(0), iniY(0), dirX(1), dirY(0),
	frameRow(frameRow), frameCol(frameCol), sizeX(juego->getCellSize(0)), sizeY(juego->getCellSize(1)){}
GameCharacter::GameCharacter(Game*juego, Texture* texture, unsigned int frameRow, unsigned int frameCol, unsigned int posX, unsigned int posY)
	: GameObject(juego), texture(texture), posX(posX), posY(posY), iniX(posX), iniY(posY), dirX(1), dirY(0),
	frameRow(frameRow), frameCol(frameCol), sizeX(juego->getCellSize(0)), sizeY(juego->getCellSize(1)){}


void GameCharacter::render()
{
	//Creamos el rectángulo destino
	SDL_Rect destRect = { posX * juego->getCellSize(0), posY * juego->getCellSize(1), 
		sizeX, sizeY };

	//Para que se pinte en medio de la celda
	destRect.x += ((juego->getCellSize(0) - sizeX) / 2);
	destRect.y += ((juego->getCellSize(1) - sizeY) / 2);

	texture->renderFrame(juego->getRenderer(), destRect, frameRow, frameCol);
}

void GameCharacter::loadFromFile(ifstream& file)
{
	file >> posY >> posX >> iniY >> iniX >> dirX >> dirY;
}

void GameCharacter::saveToFile(ofstream& file)
{
	file << posY << " " << posX << " " << iniY << " " << iniX << " " << dirX << " " << dirY << " ";
}

GameCharacter::~GameCharacter()
{

}
