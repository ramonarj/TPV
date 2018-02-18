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
	//Posición del HUD y dimensiones
	int posX, posY;
	int width, height;
	//Punteros
	Texture* textures[HUD_TEXTURES];
	Game* juego;
	//Array de rectángulos (uno por cada elemento del HUD)
	SDL_Rect rect[HUD_TEXTURES];
	//Para que parpadee el 1UP
	unsigned int scoreFrame;

	//Renderiza la puntuación
	void renderScore(int number);
	//Método auxiliar para los botones
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
	//Renderiza el menú
	void renderMenu();
	//Renderiza las imágenes de "win" o "gameover"
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

