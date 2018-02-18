#pragma once
#include "Texture.h"
#include <string>
#include "checkML.h"

using namespace std;
class Game;

const unsigned int ENERGY_TIME = 50;

//Definición de la clase
class Pacman
{
	//Atributos privados
private:
	//Posiciones
	int posX, posY;
	unsigned int iniX, iniY;

	//Dirección y buffer
	int dirX, dirY;
	int bufferX, bufferY;

	//Para las animaciones
	unsigned int textRow, textCol;
	//Variables del juego
	unsigned int numLives, energyLeft;
	//Punteros a su textura y al juego
	Texture* texture;
	Game* juego;
	//Dice si el buffer está vacío
	bool const bufferVacio(){ return (bufferX == 0 && bufferY == 0); };
	//Mueve a PacMan
	void move(unsigned int x, unsigned int y);

	//Métodos públicos
public:
	Pacman();
	//Constructora buena
	Pacman(Texture* texture, Game* juego, unsigned int posX = 0, unsigned int posY = 0);
	//Pinta
	void const render();
	//Se mueve
	void update();
	//Muere
	void die();
	//Cambia la dirección del buffer
	void setDir(int x, int y);
	//Dice si tenemos el power-up o no
	bool const powerUpActive(){ return energyLeft > 0; };
	unsigned int const timeLeft(){ return energyLeft; };
	//Anima a Pacman
	void animation();
	//Devuelve el nº de vidas
	unsigned int const livesLeft(){ return numLives; };
	//Añade una vida
	void addLife(){ numLives++; };
	//Comprueba las colisiones con los fantasmas
	bool checkCollisions();
	//Destructora
	~Pacman();
};