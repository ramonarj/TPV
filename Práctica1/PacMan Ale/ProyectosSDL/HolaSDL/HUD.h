#pragma once

#include "Texture.h"


class Game;
class Pacman;

using namespace std;

const int NUM_TEXTURES_VIDAS = 3;
const int NUM_TEXTURES_NORMALES = 5;
const int NUM_TEXTURES_PUNTOS = 10;

class HUD
{
private:
	enum Names {ReadyText, FondoMenuText, GameOverText, WinText, PuntuacionText};
	Texture* textures[NUM_TEXTURES_NORMALES];
	Texture* textVidas[NUM_TEXTURES_VIDAS];
	Texture* textPuntos[NUM_TEXTURES_PUNTOS];
	Game* juego;

public:
	HUD(Texture* tVida, Texture* tMenu, Texture* tReady, Texture* tGameOver, Texture* tWin, Texture* tPuntuacion, Texture* tPuntos, Game* game);
	HUD();
	~HUD();

	//1. METODOS PARA RENDERIZAR LAS TEXTURAS DEL HUD
	void renderMensaje(SDL_Renderer* renderer, bool gameOver, bool win);
	void renderMenu(SDL_Renderer* renderer);
	void renderVidas(SDL_Renderer* renderer, int numVidas);
	void renderPuntuacion(SDL_Renderer* renderer);
	void renderPuntos(SDL_Renderer* renderer, int puntos);
};

