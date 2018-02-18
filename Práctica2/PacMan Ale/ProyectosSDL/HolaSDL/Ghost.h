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

//Definici�n de la clase
class Ghost : public GameCharacter
{
	//Atributos privados
protected:
	//N� de fantasma 
	unsigned int ghostNo;

	//M�todos privados
	//Anima al fantasma
	void animate();
	//Filtra las direcciones para quedarse solo con las que pueda tomas
	void const filtraDirs(vector<Par> & dirs);
	//Elimina la direcci�n contraria a la que lleva del array
	void const deleteBackDir(vector<Par> & dirs);

	//M�todos p�blicos
public:
	//Constructora por defecto
	Ghost();
	//Constructora
	Ghost(Texture* texture, Game* juego, unsigned int ghostNo);
	Ghost(Texture* texture, Game* juego, unsigned int ghostNo, unsigned int posX, unsigned int posY);
	//M�todos heredados
	virtual void update();

	//Determina si una posici�n es posible
	virtual bool posPosible(int x, int y);

	~Ghost();
};