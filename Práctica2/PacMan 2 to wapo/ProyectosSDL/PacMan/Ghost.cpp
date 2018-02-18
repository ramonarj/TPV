//Dependencias
#include "Ghost.h"
#include "Game.h" 
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

Ghost::Ghost()
//Secuencia de inicialización
	:posX(0), posY(0), ghostNo(0), iniX(0), iniY(0), dirX(0), dirY(0), textRow(0),
	textCol(0), texture(nullptr), juego(nullptr){}

Ghost::Ghost(Texture* texture, Game* juego, unsigned int ghostNo, unsigned int posX, unsigned int posY)
//Secuencia de inicialización
	:posX(posX), posY(posY), ghostNo(ghostNo), iniX(posX), iniY(posY), dirX(1), dirY(0), textRow(0),
	textCol(ghostNo * 2), texture(texture), juego(juego){}

void const Ghost::render()
{
	//Creamos el rectángulo y dibujamos
	SDL_Rect destRec = { posX * juego->getCellSize(0), posY * juego->getCellSize(1),
		juego->getCellSize(0), juego->getCellSize(1) };

	texture->renderFrame(juego->getRenderer(), destRec, textRow, textCol);
}

void Ghost::update()
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
		//Elegimos la dirección aleatoriamente 
		int random = rand() % dirs.size();
		dirX = dirs[random].x;
		dirY = dirs[random].y;
	}
	//Si no hay ninguna dirección disponible, no se mueve
	else
		dirX = dirY = 0;

	//4.Movemos al fantasma con la seguridad de que esa casilla es posible
	posX += dirX;
	posY += dirY;
	juego->compruebaLimites(posX, posY);
	juego->nextCell(posX, posY); //Tenemos en cuenta el toroide (para eso llamamos a nextCell)

	//5.Animamos al fantasma
	animate();
}

void const Ghost::filtraDirs(vector<Par> &dirs)
{
	//Quitamos las posiciones a las que el fantasma se pueda mover
	unsigned int cont = 0;
	unsigned int aux;
	for (int i = 0; i < 4; i++)
	{
		int nx = posX + dirs[cont].x;
		int ny = posY + dirs[cont].y;
		juego->compruebaLimites(nx, ny);
		if (juego->nextCell(nx, ny) && !juego->hayFantasma(nx, ny, aux))
			cont++;
		else
			dirs.erase(dirs.begin() + cont);
	}

	//Si hay más de una dirección posible, quitamos la contraria a la actual
	if (dirs.size() > 1)
		deleteBackDir(dirs);
}

void const Ghost::deleteBackDir(vector<Par> &dirs)
{
	unsigned int j = 0;
	Par dirActual;
	dirActual.x = dirX;
	dirActual.y = dirY;

	//Buscamos la dirección contraria
	while (j < dirs.size() && !Game::ParesOpuestos(dirActual, dirs[j]))
		j++;

	//Si la dirección contraria se encuentra, la borramos
	if (j < dirs.size())
		dirs.erase(dirs.begin() + j);
}

void Ghost::die()
{
	posX = iniX;
	posY = iniY;
}

void Ghost::animate()
{
	//Cambiamos las animaciones dependiendo del tiempo de power-up que quede
	//1.HAY POWER-UP
	if (juego->powerUpActive())
	{
		//a)Queda tiempo todavía
		if (juego->timeLeft() > ENERGY_TIME / 4)
		{
			textRow = 0;
			if (!juego->alterna(textCol, 12, 13))
				textCol = 12;
		}
		//b)Queda poco tiempo
		else
		{
			//Columna
			juego->alterna(textCol, 12, 13);
			//Fila
			juego->alterna(textRow, 0, 1);
		}
	}
	//2.NO HAY POWER-UP
	else
	{
		//Columna(cambia en cada frame)
		if (!juego->alterna(textCol, ghostNo * 2, ghostNo * 2 + 1))
			textCol = ghostNo * 2;	

		//Fila (depende de la dirección): derecha, izquierda, arriba o abajo (en ese orden)
		if (dirX == 1)
			textRow = 0;
		else if (dirX == -1)
			textRow = 2;
		else if (dirY == -1)
			textRow = 3;
		else
			textRow = 1;
	}
}

Ghost::~Ghost()
{

}



