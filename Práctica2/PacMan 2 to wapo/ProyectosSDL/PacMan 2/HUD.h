//Dependencias
#pragma once
#include "Texture.h"
#include "checkML.h"

using namespace std;
class Game;

const unsigned int HUD_TEXTURES = 14;

class HUD
{
private:
	//Posici�n del HUD y dimensiones
	int posX, posY;
	int width, height;
	//Punteros
	Texture* textures[HUD_TEXTURES];
	Game* juego;
	//Array de rect�ngulos (uno por cada elemento del HUD)
	SDL_Rect rect[HUD_TEXTURES];
	//Para que parpadee el 1UP
	unsigned int scoreFrame;

	//Renderiza la puntuaci�n
	void renderScore(int number);
	//M�todo auxiliar para los botones
	bool insideRect(int x, int y, SDL_Rect rect);
	//Renderiza la pantalla de puntuaciones
	void renderHighScores();
	void puntuaciones(string filename);

	
public:
	//Constructora por defecto
	HUD();
	//Constructora buena
	HUD(unsigned int width, unsigned int height, Texture* textures[], Game* juego);
	//Renseriza el HUD in-game (vidas y puntos)
	void render();
	//Renderiza el men�
	void renderMenu();
	//Renderiza las im�genes de "win" o "gameover"
	void renderEspecial(bool winGame);
	//Renderiza el "Ready"
	void renderReady();
	//Renderiza la pausa para guardar la partida
	void renderPausa();
	void renderCode(int code);
	//Para elegir el tipo de partida
	string chooseGame();
	//Destructora
	~HUD();
};

