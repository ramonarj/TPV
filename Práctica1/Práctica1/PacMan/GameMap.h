//Dependencias
#pragma once
#include <vector>
#include "Texture.h"
#include <string>
#include "SDL.h"
#include "checkML.h"

using namespace std;

enum MapCell { Empty, Wall, Food, Vitamins };

//Definición de la clase
class GameMap
{
//Atributos privados
private:
	MapCell** cells;
	Texture *muro, *comida, *vitamina;
	unsigned int numRows, numCols;
	int vitaminFrame;
//Métodos públicos
public:
	//Constructora por defecto
	GameMap();
	//Constructora buena
	GameMap(unsigned int numRows, unsigned int numCols, Texture* muro, Texture* comida, Texture* vitamina);
	//Renderiza
	void render(SDL_Renderer* renderer, unsigned int cellWidth, unsigned int cellHeight);
	//Getter y setter de las celdas
	void setCell(unsigned int row, unsigned int col, MapCell cell){ cells[row][col] = cell; };
	MapCell getCell(unsigned int row, unsigned int col){ return cells[row][col]; };
	//Getter del nº de filas(i=0)/columnas(i=1)
	int const getSize(int i){ if (i == 0) return numRows; else return numCols; }
	//Destructora
	~GameMap();
};

