#include "GameMap.h"
#include "Game.h"
#include <iostream>


GameMap::GameMap()
//Secuencia de inicialización
: numRows(0), numCols(0), muro(nullptr), comida(nullptr), vitamina(nullptr), vitaminFrame(0){}


GameMap::GameMap(unsigned int numRows, unsigned int numCols,Texture* muro, Texture* comida, Texture* vitamina)
//Secuencia de inicialización
	: numRows(numRows), numCols(numCols), muro(muro), comida(comida), vitamina(vitamina), vitaminFrame(0)
{
	//Inicialización de la matriz
	cells = new MapCell*[numRows];
	for (unsigned int i = 0; i < numRows; i++)
		cells[i] = new MapCell[numCols];
}


void GameMap::render(SDL_Renderer* renderer, unsigned int cellWidth, unsigned int cellHeight)
{
	//Cambiamos el bit de las vitaminas
	Game::alterna(vitaminFrame, 0, 1);

	//Hacemos el rectángulo
	SDL_Rect destRect = { 0, 0, cellWidth, cellHeight };
	//De momento está probado en consola, hasta que tenga todas las texturas cargadas
	for (unsigned int i = 0; i < numRows; i++)
	{
		for (unsigned int j = 0; j < numCols; j++)
		{
			destRect.x = j*destRect.w;
			destRect.y = i*destRect.h;
			if (cells[i][j] == Food)
				comida->render(renderer, destRect);
			else if (cells[i][j] == Wall)
				muro->render(renderer, destRect);
			else if (cells[i][j] == Vitamins)
				vitamina->renderFrame(renderer, destRect, 0, vitaminFrame);
		}
	}
}

GameMap::~GameMap()
{
	if (cells != nullptr)
	{
		for (unsigned int i = 0; i < numRows; i++)
			delete[] cells[i];
		delete[]cells;
	}
}