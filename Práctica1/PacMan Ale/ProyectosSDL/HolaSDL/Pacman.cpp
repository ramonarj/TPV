#include "Pacman.h"
#include "Game.h"

Pacman::Pacman()
{
}

//Constructora del PACMAN
Pacman::Pacman(Texture* texture, int x, int y, Game* game)
{
	setPos(y, x);
	setIni(y, x);
	setDir(1, 0);
	setBuff(1, 0);
	textRow = 0; textCol = 10;
	juego = game;
	vidas = 3;
	textPacman = texture;
}


Pacman::~Pacman()
{
}

//1. METODOS BASICOS PARA EL FUNCIONAMIENTO DEL PACMAN
//1.1 Metodo para determinar el rectangulo destino y llamar al renderFrame de la clase Texture
void Pacman::render(SDL_Renderer* renderer)
{
	SDL_Rect destRect;
	destRect.x = posX * juego->getFrameW();
	destRect.y = posY * juego->getFrameH();
	destRect.w = juego->getFrameW();
	destRect.h = juego->getFrameH();
	textPacman->renderFrame(renderer, destRect, textRow, textCol);
}

//1.2 Metodo para modificar la posicion del Pacman, y cambiar la casilla y el numero de comida restante si es preciso
void Pacman::update()
{
	nx = posX + bufferX;
	ny = posY + bufferY;

	if (juego->nextCell(nx, ny))
	{
		setDir(bufferX, bufferY);
	}
	else
	{
		nx = posX + dirX;
		ny = posY + dirY;
	}

	if (juego->nextCell(nx, ny))
	{
		setPos(nx, ny);
		if (juego->getCasilla(posY, posX) == Food || juego->getCasilla(posY, posX) == Vitamins)
		{
			juego->setNumComida(-1);
			juego->cambiaCell(posY, posX);
		}
	}
	animation();
}

//1.3 Metodo que realiza la animacion del pacman
void Pacman::animation()
{
	if (dirX == 1 && dirY == 0)
		textRow = 0;
	else if (dirX == -1 && dirY == 0)
		textRow = 2;
	else if (dirX == 0 && dirY == 1)
		textRow = 1;
	else
		textRow = 3;

	if (textCol == 10) textCol = 11;
	else textCol = 10;
}

//2. METODOS QUE CONTROLAN LAS VIDAS DEL PACMAN
//2.1 Resta vida al pacman y lo devuelve a su posicion inicial
void Pacman::lifes()
{
	vidas--;
	setDir(1, 0);
	setBuff(1, 0);
	setPos(iniX, iniY);
}


