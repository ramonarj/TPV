#include "GameMap.h"
#include "Game.h"
#include <iostream>


GameMap::GameMap()
//Secuencia de inicialización
: numRows(0), numCols(0), muro(nullptr), comida(nullptr), vitamina(nullptr), foodLeft(0), vitaminFrame(0){}


GameMap::GameMap(Texture* muro, Texture* comida, Texture* vitamina, Game*juego)
//Secuencia de inicialización (con la constructora del padre)
: GameObject(juego), numRows(0), numCols(0), muro(muro), comida(comida), vitamina(vitamina), foodLeft(0), vitaminFrame(0){}


void GameMap::render()
{
	//Cambiamos el bit de las vitaminas
	Game::alterna(vitaminFrame, 0, 1);

	//Hacemos el rectángulo
	SDL_Rect destRect = { 0, 0, juego->getCellSize(0), juego->getCellSize(1) };
	//De momento está probado en consola, hasta que tenga todas las texturas cargadas
	for (unsigned int i = 0; i < numRows; i++)
	{
		for (unsigned int j = 0; j < numCols; j++)
		{
			destRect.x = j*destRect.w;
			destRect.y = i*destRect.h;
			if (cells[i][j] == Food)
				comida->render(juego->getRenderer(), destRect);
			else if (cells[i][j] == Wall)
				muro->render(juego->getRenderer(), destRect);
			else if (cells[i][j] == Vitamins)
				vitamina->renderFrame(juego->getRenderer(), destRect, 0, vitaminFrame);
		}
	}
}

void GameMap::update()
{

}

void GameMap::loadFromFile(ifstream& file)
{
	//Nºfilas y columnas
	file >> numRows >> numCols;

	//Creamos y rellenamos la matriz
	cells = new MapCell*[numRows];
	//Filas 
	unsigned int i = 0;
	while (i < numRows && !file.fail())
	{
		cells[i] = new MapCell[numCols];
		//Columnas
		unsigned int j = 0;
		while (j < numCols && !file.fail())
		{
			int valor;
			file >> valor;
			fillPos(i, j, valor);
			j++;
		}
		i++;
	}
}

void GameMap::fillPos(unsigned int i, unsigned int j, unsigned int valor)
{
	//Vacío
	if (valor == 0)
		cells[i][j] = Empty;
	//Muro
	else if (valor == 1)
		cells[i][j] = Wall;	
	//Comida
	else if (valor == 2)
	{
		cells[i][j] = Food;
		foodLeft++;
	}
	//Vitamina
	else if (valor == 3)
	{
		cells[i][j] = Vitamins;
		foodLeft++;
	}
	//else: lanzar excepción
}

void GameMap::writePos(MapCell valor, ofstream& file)
{
	//Vacío
	if (valor == Empty)
		file << 0;
	//Muro
	else if (valor == Wall)
		file << 1;
	//Comida
	else if (valor == Food)
		file << 2;
	//Vitamina
	else if (valor == Vitamins)
		file << 3;
	else
		cout << valor;
	file << " ";
}

void GameMap::eatFood(unsigned int row, unsigned int col)
{
	cells[row][col] = Empty;
	foodLeft--;
	if (foodLeft < 1)
		juego->Win();
}

void GameMap::saveToFile(ofstream&file)
{
	//Filas y columnas
	file << numRows << " " << numCols << endl;

	//Valor de las celdas
	unsigned int i = 0;
	while (i< numRows && !file.fail())
	{
		unsigned int j = 0;
		while (j< numCols && !file.fail())
		{
			writePos(cells[i][j], file);
			j++;
		}
		file << endl;
		i++;
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