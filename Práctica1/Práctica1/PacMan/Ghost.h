//Dependencias
#pragma once
#include "Texture.h"
#include <string>
#include <vector>
#include "checkML.h"

using namespace std;
class Game;

//Estructura para las direcciones
struct Par
{
	int x = 0;
	int y = 0;
};
//Definición de la clase
class Ghost
{
//Atributos privados
private:
	//Posiciones
	int posX, posY;
	unsigned int iniX, iniY;

	//Dirección
	int dirX, dirY;

	//Nº de fantasma y posición del frame
	unsigned int ghostNo;
	int textRow, textCol;

	//Punteros a su textura y al juego
	Texture* texture;
	Game* juego;

	//Métodos privados
	//Anima al fantasma
	void animate();
	//Filtra las direcciones para quedarse solo con las que pueda tomas
	void const filtraDirs(vector<Par> & dirs);
	//Elimina la dirección contraria a la que lleva del array
	void const deleteBackDir(vector<Par> & dirs);
	
//Métodos públicos
public:
	//Constructora por defecto
	Ghost();
	//Constructora
	Ghost(Texture* texture, Game* juego, unsigned int ghostNo, unsigned int posX = 0, unsigned int posY=0);
	//Renderiza
	void const render();
	//Actualiza la lógica
	void update();
	//Muere
	void die();
	//Devuelve las posiciones de los fantasmas
	unsigned int const getPosX(){ return posX; };
	unsigned int const getPosY(){ return posY; };

	~Ghost();
};