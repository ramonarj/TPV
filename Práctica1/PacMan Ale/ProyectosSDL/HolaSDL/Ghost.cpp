#include "Ghost.h"
#include "Game.h"
#include <iostream>

Ghost::Ghost()
{
}

//Constructora del GHOST
Ghost::Ghost(Texture* texture, int x, int y, int num, Game* game)
{
	setPos(y, x);
	setIni(y, x);
	setDir(0, 0);
	ghostNumber = num;
	textRow = 0; textCol = ghostNumber * 2;

	juego = game;
	textGhost = texture;
}


Ghost::~Ghost()
{
}

//1. METODOS PARA EL FUNCIONAMENTO BASSICO DE LOS FANTASMAS
//1.1 Metodo para determinar el rectangulo destino y llamar al renderFrame de la clase Texture
void Ghost::render(SDL_Renderer* renderer)
{
	SDL_Rect destRect;
	destRect.x = posX * juego->getFrameW();
	destRect.y = posY * juego->getFrameH();
	destRect.w = juego->getFrameW();
	destRect.h = juego->getFrameH();
	textGhost->renderFrame(renderer, destRect, textRow, textCol);
}

//1.2 Metodo para mover al fantasma
void Ghost::update()
{
	if (!muriendo)
	{
		seleccionaDir();
		int nx = posX + dirX, ny = posY + dirY;
		if (juego->nextCell(nx, ny))
			setPos(nx, ny);
	}
	animation();
}

//1.3 Metodo para matar al fantasmas
void Ghost::dead() 
{ 
	if (!muriendo)
		setPos(iniX, iniY);
}

//1.4 Metodo que realiza la animacion del pacman
void Ghost::animation()
{
	if (!juego->getPower() && !muriendo)
	{
		if (dirX == 1 && dirY == 0)
			textRow = 0;
		else if (dirX == -1 && dirY == 0)
			textRow = 2;
		else if (dirX == 0 && dirY == 1)
			textRow = 1;
		else
			textRow = 3;
		if (textCol == ghostNumber * 2) textCol = (ghostNumber * 2) + 1;
		else textCol = ghostNumber * 2;
	}
	else if (juego->getPower() && !muriendo)
	{
		if (juego->getCont() < (POWER_TIME - (POWER_TIME / 4)))
			textRow = 0;
		else
		{
			if (textRow == 0) textRow = 1;
			else textRow = 0;
		}
		if (textCol == 12) textCol = 13;
		else textCol = 12;
	}
	else if ((!juego->getPower() || juego->getPower()) && muriendo)
	{
		textRow = 2;
		if (textCol == 12) textCol = 13;
		else
		{
			textRow++;
			textCol = 12;
		}
		if (textRow == 3) muriendo = false;
		dead();
	}
}

//2. METODOS PARA EL MOVIMIENTO DE LOS FANTASMAS
//2.1 Calcula las posibles direcciones de movimiento del fantasma
//Izquierda: (-1, 0); Derecha: (1, 0); Arriba: (0, -1); Abajo: (0, 1)
void Ghost::posiblesDirs()
{
	rellenaDirs(1, 0);
	rellenaDirs(0, 1);
	rellenaDirs(-1, 0);
	rellenaDirs(0, -1);

	if (direcciones.size() > 1)
		eliminaDirContraria();
}

//2.2 Metodo para rellenar el vector de Pares
void Ghost::rellenaDirs(int x, int y)
{
	pares dir;
	int nx = posX + x;
	int ny = posY + y;
	if (juego->nextCell(nx, ny) && !juego->hayFantasma(nx, ny))
	{
		dir.x = x; dir.y = y;
		direcciones.push_back(dir);
	}
}

//2.3 Selecciona una direccion aleatoria entre todas las posibles
void Ghost::seleccionaDir()
{
	posiblesDirs();
	int n;
	if (direcciones.size() != 0)
	{
		n = rand() % direcciones.size();
		setDir(direcciones[n].x, direcciones[n].y);
	}
	else
		setDir(0, 0);
	direcciones.clear();
}

//2.4 Metodo que elimina la posicion contraria del fantasma para que no de la vuelta
void Ghost::eliminaDirContraria()
{
	int i = 0;
	bool eliminado = false;
	while (i < direcciones.size() && !eliminado)
	{
		if (dirX * (-1) == direcciones[i].x && dirY * (-1) == direcciones[i].y)
		{
			direcciones.erase(direcciones.begin() + i);
			eliminado = true;
		}
		else
			i++;
	}
}

void Ghost::muriendoTrue()
{ 
	muriendo = true; 
	textRow = 1;
	textCol = 12;
}

