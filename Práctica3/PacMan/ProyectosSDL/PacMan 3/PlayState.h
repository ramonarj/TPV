#pragma once
#include "GameState.h"
#include "Pacman.h"
#include "GameMap.h"
#include "HUD.h"


class PlayState: public GameState
{
private:
	Pacman* pac;
	GameMap* map;

	bool gameover, win;
	int score = 0;
	int cellWidth, cellHeight;
	int level = 1;
	string filename;


	//M�TODOS PRIVADOS:
	//Borra a los personajes para pasar de nivel
	void deleteGhosts();

	//Lee el mapa
	void loadMap(string filename);
public:
	PlayState();
	PlayState(Game* juego, HUD* hud, string filename);

	virtual void render();
	virtual void update();
	virtual void handleEvent(SDL_Event& e);

	//M�TODOS QUE ANTES TEN�A GAME
	//Pregunta por una celda y cambia las posiciones en caso de que se salgan del mapa
	bool nextCell(int x, int y);
	//Pacman se come la comida de una posici�n
	bool eatFood(int x, int y);
	//Suma puntos a la puntuaci�n actual
	void addScore(unsigned int i){ score += i; };
	//Devuelve la puntuaci�n actual
	int getScore(){ return score; };
	//Nos dice la puntuaci�n actual
	void killGhost(list<GameObject*>::iterator it);


	//Mata a Pacman
	void killPacman();
	//"Termina" la partida
	void setgameOver(){ gameover = true; };
	//"Termina" la partida
	bool getgameOver(){ return gameover; };
	//Dice si el power-up est� activo o no
	bool const powerUpActive(){ return pac->powerUpActive(); };
	unsigned int const timeLeft(){ return pac->timeLeft(); };
	//Nos dice las vidas restantes
	unsigned int const livesLeft(){ return pac->livesLeft(); };
	//Indica si hay fantasma o no en esa posici�n
	bool hayFantasma(int x, int y, list<GameObject*>::iterator & it);

	//Completa el nivel
	void Win(){ win = true; };
	//Devuelve la posici�n de Pacman(para los smartGhosts)
	int getPacPos(int i){ if (i == 0) return pac->getPosX(); else return pac->getPosY(); };
	//Crea un beb� smartGhost en la posici�n dada
	void createBabyGhost(unsigned int x, unsigned int y);
	//Devuelve el tama�o del mapa
	int const getSize(int i){ return map->getSize(i); };
	//Comprueba que x e y est�n en los l�mites del mapa, recoloc�ndolos si es necesario
	void const compruebaLimites(int &x, int &y);
	//Devuelven las dimensiones de las celdas (0=Width, 1=Height)
	int getCellSize(unsigned int i){ if (i == 0) return cellWidth; else return cellHeight; };

	//Guarda la partida actual
	void SaveGame(int code);

};

