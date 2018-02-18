#pragma once

#include "SDL.h"
#include <string>
#include <iostream>

using namespace std;

//Para el mapa
enum MapCell {Empty, Wall, Food, Vitamins};

//Dependencias
class Texture;
class Game;


class GameMap
{
private:
	Texture* muro, *comida, *vitamina;

	//Tamaño del mapa
	int fils, cols;
	int texColVit;
	MapCell **matrizJuego;

public:
	GameMap();
	GameMap(Texture* tMuro, Texture* tComida, Texture* tVitamina, Game* game);
	~GameMap();

	//1. METODOS PARA EL FUNCIONAMIENTO BASICO 
	void render(SDL_Renderer* renderer, Game* game);

	//2. METODOS PARA CREAR EL MAPA
	void creaMatrizVacia(int x, int y);
	void creaMapa(int n, int i, int j, Game* game);

	//3. METODOS PARA COMPROBARUNA POSICION DETERMINADA
	bool consultaPos(int x, int y);
	void escribePos(int x, int y, Game* game);

	//4. GETTERS Y SETTERS
	int getFils() { return fils; };
	int getCols() { return cols; };
	MapCell getCell(int x, int y) { return matrizJuego[x][y]; };
};

