#include "Pacman.h"
#include "Game.h"
#include <iostream>
#include <typeinfo>

Pacman::Pacman()
//Secuencia de inicialización
	:GameCharacter(), numLives(0), energyLeft(0), bufferX(0), bufferY(0) {}


Pacman::Pacman(Texture* texture, Game* juego, unsigned int posX, unsigned int posY)
//Secuencia de inicialización
	: GameCharacter(juego, texture, 2, 11), numLives(3), energyLeft(0), bufferX(0), bufferY(0) {}

void Pacman::loadFromFile(ifstream& file)
{
	GameCharacter::loadFromFile(file);
	energyLeft = 0;

	int aux;
	file >> aux;
	//No era partida guardada
	if (file.fail())
		file.clear();
	//Sí lo era
	else
	{
		energyLeft = aux;
		file >> numLives;
	}
	frameRow = 2;
	frameCol = 11;
}

void Pacman::update()
{
	//0.COMPRUEBA COLISIONES
	checkCollisions();

	if (!juego->getgameOver())
	{
		//1.RESTAMOS ENERGÍA
		if (energyLeft > 0)
			energyLeft--;

		//2.MOVEMOS A PACMAN
		//Miramos si podemos coger la dirección del buffer
		int nx = posX + bufferX;
		int ny = posY + bufferY;
		juego->compruebaLimites(nx, ny);

		//Vemos si podemos coger esa dirección
		if (juego->nextCell(nx, ny) && !bufferVacio())
		{
			//Cogemos la dirección
			dirX = bufferX;
			dirY = bufferY;
			//Vaciamos el buffer
			setDir(0, 0);
			//Movemos
			move(nx, ny);
		}

		//Si no, cogemos la actual
		else
		{
			nx = posX + dirX;
			ny = posY + dirY;
			juego->compruebaLimites(nx, ny);
			if (juego->nextCell(nx, ny))
				move(nx, ny);
		}

		//3.SE COME LA COMIDA (si hay)
		if (juego->eatFood(posX, posY))
			energyLeft = ENERGY_TIME; //(Y la vitamina)

									  //4.VUELVE A COMPROBAR COLISIONES
		checkCollisions();
	}
}

void Pacman::move(unsigned int x, unsigned int y)
{
	posX = x;
	posY = y;
	animation();
}

void Pacman::die()
{
	//Le quitamos una vida y llamamos a GameOver si es necesario
	numLives--;
	if (numLives < 1)
		juego->setgameOver();
	else
	{
		//Lo movemos a la posición y dirección de inicio
		GameCharacter::die();
		dirX = 1;
		dirY = 0;
		frameRow = 0;
		frameCol = 11;
	}
}

void Pacman::animation()
{
	//Columna (cambia en cada frame)
	if (frameCol == 10)
		frameCol++;
	else
		frameCol--;

	//Fila (depende de la dirección)
	if (dirX == 1)
		frameRow = 0;
	else if (dirX == -1)
		frameRow = 2;
	else if (dirY == -1)
		frameRow = 3;
	else
		frameRow = 1;
}

bool Pacman::checkCollisions()
{
	list<GameCharacter*>::iterator it;
	//Si hay un fantasma en la misma casilla que pacman
	if (juego->hayFantasma(posX, posY, it))
	{
		SmartGhost* smartGhost = dynamic_cast<SmartGhost*>(*it);
		//Mata al fantasma...
		if (energyLeft > 0 || (smartGhost != nullptr && smartGhost->isDead()))
			juego->killGhost(it);

		//...o muere él
		else
			juego->killPacman();
		return true;
	}
	else
		return false;
}

void Pacman::saveToFile(ofstream& file)
{
	GameCharacter::saveToFile(file);
	file << energyLeft << " " << numLives;
}

Pacman::~Pacman()
{

}