#pragma once
#include "Texture.h"
#include "GameObject.h"
#include "checkML.h"

using namespace std;

class GameCharacter:public GameObject
{
protected:
	//Puntero a su textura
	Texture* texture;
	//Posici�n y direcci�n
	int posX, posY;
	int iniX, iniY;
	int dirX, dirY;
	//Posici�n del frame y tama�o al que hay que dibujarlo
	int frameRow, frameCol;
	unsigned int sizeX, sizeY;

	GameCharacter();
	GameCharacter(Game* juego, Texture* texture, unsigned int frameRow, unsigned int frameCol);
	GameCharacter(Game* juego, Texture* texture, unsigned int frameRow, unsigned int frameCol, unsigned int posX, unsigned int posY);
public:
	//Hay que implementar los m�todos heredados (menos update)
	virtual void render();
	virtual void loadFromFile(ifstream& file);
	virtual void saveToFile(ofstream& file);

	//Muere
	virtual void die(){ posX = iniX; posY = iniY; };
	//Devuelve las posiciones del personaje
	unsigned int const getPosX(){ return posX; };
	unsigned int const getPosY(){ return posY; };

	//Hace grande al personaje
	virtual void grow(){ sizeX++; sizeY++; };
	virtual ~GameCharacter();
};

