#pragma once
#include "Ghost.h"
#include "checkML.h"

using namespace std;

const int ADULT_AGE = 20;
const int DEATH_AGE = 100;
const int CHILDREN_RATE = 10; //Para que no pasen cosas raras, limitamos al SmartGhost para que solo pueda tener 1 hijo cada 10 frames

class SmartGhost : public Ghost
{
private:
	unsigned int age;
	bool dead;
	unsigned int childrenTime; //Empieza en 0, y se va decrementando al tener un hijo

							   //Se mueve de forma inteligente
	void smartMove();
	//Determina qué posición es mejor
	bool mejor(Par par1, Par par2);
	//Intenta ligar con otro smartGhost
	void flirt();
	//Dice si en determinado sitio se puede procrear o no
	bool emptyPlace(Par& babyPos);

public:
	SmartGhost();
	SmartGhost(Texture* texture, Game* juego);
	SmartGhost(Texture* texture, Game* juego, unsigned int posX, unsigned int posY); //Para las relaciones entre SmartGhosts
	virtual void loadFromFile(ifstream& file);
	virtual void saveToFile(ofstream& file);
	virtual void die();
	virtual void update();

	//El fantasma está muerto
	bool const isDead() { return dead; };
	//El fantasma es adulto o no
	bool const isFertile() { return (age > ADULT_AGE && age < DEATH_AGE) && (childrenTime >= CHILDREN_RATE); };
	//El fantasma tiene un hijo
	void haveChild() { childrenTime = 0; };


	~SmartGhost();
};

