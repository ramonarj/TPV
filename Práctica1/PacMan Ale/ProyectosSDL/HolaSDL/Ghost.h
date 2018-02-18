#pragma once

#include "Texture.h"
#include <vector>

using namespace std;

//Dependencias
class Game;

struct  pares
{
	int x;
	int y;
};

class Ghost
{
private:
	int iniX, iniY;	//Posiciones iniciales
	int posX, posY;	//Posiciones actuales
	int dirX, dirY;	//Direcciones actuales de movimiento
	int textRow, textCol;
	int ghostNumber;
	bool muriendo = false;
	vector <pares> direcciones;
	Texture* textGhost;
	Game* juego;

public:
	Ghost();
	Ghost(Texture* texture, int x, int y, int num, Game* game = nullptr);
	~Ghost();

	//1. METODOS PARA EL FUNCIONAMENTO BASSICO DE LOS FANTASMAS
	void render(SDL_Renderer* renderer);
	void update();
	void dead();
	void animation();

	//2. METODOS PARA EL MOVIMIENTO DE LOS FANTASMAS
	void posiblesDirs();
	void rellenaDirs(int nx, int ny);
	void seleccionaDir();
	void eliminaDirContraria();

	//3. GETTERS Y SETTERS
	void setPos(int x, int y) { posX = x; posY = y; };
	int getPosX() { return posX; };
	int getPosY() { return posY; };
	
	void setIni(int x, int y) { iniX = x; iniY = y; };

	void setDir(int x, int y) { dirX = x; dirY = y; };

	void muriendoTrue();
};

