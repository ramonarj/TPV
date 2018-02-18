//Dependencias
#pragma once
#include "Texture.h"
#include "checkML.h"
#include "PacManObject.h"

using namespace std;
class Game;

const unsigned int HUD_TEXTURES = 16;

class HUD: public GameObject
{
private:
	//Posición del HUD y dimensiones
	int posX, posY;
	int width, height;
	//Punteros
	Texture* textures[HUD_TEXTURES];
	//Array de rectángulos (uno por cada elemento del HUD)
	SDL_Rect rect[HUD_TEXTURES];
	//Para que parpadee el 1UP
	unsigned int scoreFrame;

	//Renderiza la puntuación
	void renderScore(int number);
	//Renderiza la pantalla de puntuaciones
	void puntuaciones(string filename);

	
public:
	//Constructora por defecto
	HUD();
	//Constructora buena
	HUD(Game* juego);
	//Renseriza el HUD in-game (vidas y puntos)
	virtual void render(int lives, int score);
	virtual void render();
	virtual void update();
	virtual bool handleEvent(SDL_Event& e){ return false; };

	//Renderiza las imágenes de "win" o "gameover"
	void renderEspecial(bool winGame);
	//Renderiza el "Ready"
	void renderReady();
	//Renderiza la pausa para guardar la partida
	void buttonPausa();
	void renderCode(int code);
	//Renderiza el menú
	void renderMenu();
	void buttonMenu();

	void changeRect(int text, int x, int y, int w, int h);

	void renderHighScores();
	void buttonEnd();

	SDL_Rect getRect(int i) { return rect[i]; };
	//Destructora
	~HUD();
};

