//Dependencias
#pragma once
#include <vector>
#include "Texture.h"
#include <string>
#include <fstream>
#include "SDL.h"
#include "checkML.h"
#include "GameObject.h"

using namespace std;

enum MapCell { Empty, Wall, Food, Vitamins };

//Definici�n de la clase
class GameMap :public GameObject
{
//Atributos privados
private:
	MapCell** cells;
	Texture *muro, *comida, *vitamina;
	unsigned int numRows, numCols;
	int foodLeft;
	int vitaminFrame;

	//M�todos privados
	//Rellena un posici�n del mapa
	void fillPos(unsigned int i, unsigned int j, unsigned int valor);
	//Guarda una posici�n en archivo
	void writePos(MapCell valor, ofstream& file);
//M�todos p�blicos
public:
	//Constructora por defecto
	GameMap();
	//Constructora buena
	GameMap(Texture* muro, Texture* comida, Texture* vitamina, Game* juego);
	//Getter y setter de las celdas
	MapCell getCell(unsigned int row, unsigned int col){ return cells[row][col]; };
	//Getter del n� de filas(i=0)/columnas(i=1)
	int const getSize(int i){ if (i == 0) return numRows; else return numCols; }

	//M�todos heredados de GameObject
	virtual void update();
	virtual void loadFromFile(ifstream&file);
	virtual void saveToFile(ofstream& file);
	virtual void render();

	
	//Devuelve la comida restante
	int getFoodLeft(){ return foodLeft; };
	//Elimina la comida de una celda
	void eatFood(unsigned int row, unsigned int col);

	//Destructora
	virtual ~GameMap();
};

