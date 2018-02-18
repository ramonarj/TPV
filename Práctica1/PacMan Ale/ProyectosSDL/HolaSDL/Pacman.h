#pragma once

#include "Texture.h"

class Game;

using namespace std;

class Pacman
{
private:
	int iniX, iniY;	//Posiciones iniciales
	int posX, posY;	//Posiciones actuales
	int dirX, dirY;	//Direcciones actuales de movimiento
	int bufferX, bufferY;
	int nx, ny;
	int textRow, textCol;
	int vidas;
	Texture* textPacman;
	Game* juego;

public:
	Pacman();
	Pacman(Texture* texture, int x, int y, Game* game = nullptr);
	~Pacman();
	
	//1. METODOS BASICOS PARA EL FUNCIONAMIENTO DEL PACMAN
	void render(SDL_Renderer* renderer);
	void update();
	void animation();

	//2. METODOS QUE CONTROLAN LAS VIDAS DEL PACMAN
	void dead();
	void lifes();

	//3. GETTERS Y SETTERS
	void setPos(int x, int y) { posX = x; posY = y; };
	int getPosX() { return posX; };
	int getPosY() { return posY; };

	void setIni(int x, int y) { iniX = x; iniY = y; };

	void setBuff(int x, int y) { bufferX = x; bufferY = y; };
	void setDir(int x, int y) { dirX = x; dirY = y; };

	int getVidas() { return vidas; };
};

