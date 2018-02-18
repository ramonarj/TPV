//Dependencias
#pragma once
#include "Texture.h"
#include <string>
#include <vector>
#include <fstream>
#include "checkML.h"
#include "GameCharacter.h"

using namespace std;
class Game;

//Estructura para las direcciones
struct Par
{
	int x = 0;
	int y = 0;
};

//Definición de la clase
class Ghost : public GameCharacter
{
	//Atributos privados
protected:
	//Nº de fantasma 
	unsigned int ghostNo;

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
	Ghost(Texture* texture, Game* juego, unsigned int ghostNo);
	Ghost(Texture* texture, Game* juego, unsigned int ghostNo, unsigned int posX, unsigned int posY);
	//Métodos heredados
	virtual void update();

	//Determina si una posición es posible
	virtual bool posPosible(int x, int y);

	~Ghost();
};