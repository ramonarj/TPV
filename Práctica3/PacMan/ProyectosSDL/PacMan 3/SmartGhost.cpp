#include "SmartGhost.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include "PlayState.h"

SmartGhost::SmartGhost(): Ghost(), age(0), dead(false){}

SmartGhost::SmartGhost(Texture* texture, Game* juego, PlayState* playState) : Ghost(texture, juego, playState, 4), age(0), dead(false), childrenTime(0){};
SmartGhost::SmartGhost(Texture* texture, Game* juego, PlayState* playState, unsigned int posX, unsigned int posY) :
Ghost(texture, juego, playState, 4, posX, posY), age(5), dead(false), childrenTime(0) { sizeX -= (ADULT_AGE - age); sizeY -= (ADULT_AGE - age); };



void SmartGhost::loadFromFile(ifstream& file)
{
	dead = false;
	GameCharacter::loadFromFile(file);
	file >> age;
	if (age < ADULT_AGE)
	{
		sizeX -= (ADULT_AGE - age);
		sizeY -= (ADULT_AGE - age);
	}
	else if (age > DEATH_AGE)
		dead = true;
}

void SmartGhost::saveToFile(ofstream& file)
{
	GameCharacter::saveToFile(file);
	file << age;
}

void SmartGhost::update()
{
	//Si está muerto no hace nada
	if (!dead)
	{
		//Muere
		if (age > DEATH_AGE)
		{
			dead = true;
			frameRow = 1;
			frameCol = 12;
		}
			
		//Sigue viviendo
		else 
		{
			childrenTime++;
			//Crece y se mueve como un fantasma normal
			if (age < ADULT_AGE)
			{
				grow();
				Ghost::update();
			}
			//Se mueve de forma agresiva e intenta ligar(si no está activo el power-UP)
			else if (!playState->powerUpActive())
				smartMove();
						
			//Si hay power-up se mueve como un fantasma normal
			else
				Ghost::update();
			//Envejece
			age++;
		}
	}
}

void SmartGhost::smartMove()
{
	//1.Creamos el vector con las 4 direcciones
	//Izquierda, derecha, arriba y abajo (en ese orden)
	vector<Par>dirs(4);
	dirs[0].x = dirs[2].y = -1;
	dirs[1].x = dirs[3].y = 1;

	//2.Filtramos para quedarnos con las posibles (y quitando la de atrás si se da el caso)
	filtraDirs(dirs);

	//3.Elegimos una dirección
	if (dirs.size() > 0)
	{
		Par pacPos; pacPos.x = playState->getPacPos(0); pacPos.y = playState->getPacPos(1);
		Par dis; //dis = distance
		dis.x = abs(pacPos.x - posX);
		dis.y = abs(pacPos.y - posY);
		unsigned int cont = 0;
		//Vemos cuál de todas le conviene más
		for (unsigned int i = 0; i < dirs.size(); i++)
		{
			Par ndis;
			ndis.x = abs(pacPos.x - (posX + dirs[cont].x));
			ndis.y = abs(pacPos.y - (posY + dirs[cont].y));

			//Si esA dirección proporciona una mejora, la coge
			if (mejor(ndis, dis))
				cont++;

			//Si no, si no es la última dirección, la elimina
			else if (dirs.size() > 1)
				dirs.erase(dirs.begin() + cont);	
		}
		//Al final nos quedamos con la única dirección del vector (la mejor)
		dirX = dirs[0].x;
		dirY = dirs[0].y;
	}
	else //Si no hay direcciones posibles, no se mueve
		dirX = dirY = 0;

	//4.Movemos al fantasma con la seguridad de que esa casilla es posible
	posX += dirX;
	posY += dirY;
	playState->compruebaLimites(posX, posY); //Tenemos en cuenta el toroide 

	//5.Animamos al fantasma
	animate();

	//6.Intenta ligar
	flirt();
}

bool SmartGhost::mejor(Par ndis, Par dis)
{
	//Básicamente, devuelve true si la componente mayor de la distancia disminuye 
	//Ejemplo: (P=Pacman, F=fantasma). El fantasma intentará ir hacia arriba. NOTA: no tiene en cuenta el toroide
	//      P   //
	//          //
	//          //
	//  ^       //
 	//  |       //
	//  F       //
	//          //
	//La distancia inicial era (x, y), y si se mueve hacia arriba será (x, y-1) 
	//Como está más lejos en la 2º coordenada, el fantasma ve esa distancia como mejor a cualquier otra
	return((dis.x >= dis.y && ndis.x < dis.x) || (dis.y >= dis.x && ndis.y < dis.y));
}

void SmartGhost::flirt()
{
	vector<Par>dirs(4);
	dirs[0].x = dirs[2].y = -1;
	dirs[1].x = dirs[3].y = 1;

	bool flirtSuccess = false; unsigned int i = 0;
	list<GameObject*>::iterator it;
	
	//Vemos si hay algún smartGhost cerca
	while (i < dirs.size() && !flirtSuccess)
	{
		int nx = posX + dirs[i].x;
		int ny = posY + dirs[i].y;
		if (playState->hayFantasma(nx, ny, it))
		{
			Par babyPos;
			SmartGhost* smartGhost = dynamic_cast<SmartGhost*>(*it);
			//Si es un smartGhost (ADULTO), ambos son fértiles y hay sitio alrededor, procrean
			//NOTA: el otro fantasma solo intentará tener el hijo en su update si no han llamado a su haveChild() (que pone el contador a 0)
			
			if (smartGhost != nullptr && smartGhost->isFertile() && this->isFertile() && emptyPlace(babyPos))
			{
				flirtSuccess = true;

				this->haveChild(); //Esto es básicamente para qus solo puedan tener 1 hijo cuando se relacionan
				smartGhost->haveChild(); //
				playState->createBabyGhost(babyPos.x, babyPos.y);
			}	
		}
		i++;	
	}
}

bool SmartGhost::emptyPlace(Par& babyPos)
{
	vector<Par>dirs(4);
	dirs[0].x = dirs[2].y = -1;
	dirs[1].x = dirs[3].y = 1;

	list<GameObject*>::iterator it;
	bool place = false; unsigned int i = 0;
	while (i < dirs.size() && !place)
	{
		int nx = posX + dirs[i].x;
		int ny = posY + dirs[i].y;
		if (playState->nextCell(nx, ny) && !playState->hayFantasma(nx, ny, it))
		{
			babyPos.x = nx;
			babyPos.y = ny;
			place = true;
		}
		i++;
	}
	return place;
}

void SmartGhost::die()
{
	if (!this->isDead())
		GameCharacter::die();
}


SmartGhost::~SmartGhost()
{

}