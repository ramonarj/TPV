//Dependencias
#pragma once
#include "Texture.h"
#include "checkML.h"

using namespace std;
class Game;

const unsigned int HUD_TEXTURES = 8;

class HUD
{
private:
	//Posici�n del HUD y dimensiones
	int posX, posY;
	int width, height;
	//Punteros
	Texture* textures[HUD_TEXTURES];
	Game* juego;
	//Rect�ngulo para dibujarlo todo (vamos cambiando coordenadas)
	SDL_Rect rect[HUD_TEXTURES];
	//Para que parpadee el 1UP
	unsigned int scoreFrame;

	//M�todo auxiliar para renderizar n�meros en "rect"
	void renderScore(int number);
	
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
	//Destructora
	~HUD();
};

