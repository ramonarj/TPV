#pragma once

#include "SDL.h"
#include <string>
#include <vector>
#include "Pacman.h"
#include "Ghost.h"
#include "GameMap.h"
#include "HUD.h"

using namespace std;

//Constantes
struct  TextureAtributes
{
	string filename;
	int fils;
	int cols;
};
enum TextureName {CharactersText, WallText, FoodText, VitaminText, MenuText, ReadyText, GameOverText, WinText, PuntuacionText, NumerosText};
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int RENDER_TIME = 165;
const int NUM_GHOST = 4;
const int NUM_TEXTURAS = 10;
const int POWER_TIME = 20;
const string IMAGE_PATH = "..\\images\\";
const TextureAtributes TEXTURES_ATRIBUTES[NUM_TEXTURAS] = { { "characters1.png", 4, 14 }, {"wall2.png", 1, 1}, {"food2.png", 1, 1}, {"vitamin.png", 1, 2}, 
{"menu.png", 1, 1}, {"ready.png", 1, 1}, {"gameOver.png", 1, 1}, {"win.png", 1, 1}, {"puntuacion.png", 1, 1}, {"numeros.png", 1, 10} };

class Game
{
private:

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	int winX, winY;
	int frameWidth, frameHeight;

	GameMap* map;
	Pacman pacman;
	Ghost ghost[NUM_GHOST];
	HUD hud;
	
	//Booleanos de control de juego
	bool exit;
	bool gameover;
	bool win;
	bool power;
	bool ready;

	int comidaRest; //Numero de casillas restantes con comida
	string level = "..\\levels\\level01.dat";
	int contTiempo;

	Texture* texture [NUM_TEXTURAS];

public:
	Game();
	~Game();

	//1. METODOS BASICOS PARA EL FUNCIONAMIENTO
	void run();
	void render();
	void update();
	bool handleEvents();
	void end();
	void choquePersonajes();
	void contador(int max);
	void renderParado(int tiempo);

	//2. METODOS PARA CARGAR EL TABLERO (MAPA Y PERSONAJES)
	void cargaMapa(string filename);
	void crearPersonajes(int n, int i, int j);
	
	//3. METODOS PARA EL MOVIMIENTO DE LOS PERSONAJES
	bool nextCell(int& nx, int& ny);
	void toroide(int& nx, int& ny);
	void cambiaCell(int x, int y);
	bool hayFantasma(int nx, int ny);

	//4. METODOS PARA CONTROLAR SI HAS GANADO O PERDIDO
	void partidaAcabada();

	//5. GETTERS Y SETTERS
	int getFrameW() { return frameWidth; };
	int getFrameH() { return frameHeight; };
	int getFilsTab() { return map->getFils(); };
	int getColsTab() { return map->getCols(); };
	void setNumComida(int sumRest) { comidaRest += sumRest; };
	SDL_Renderer* getRenderer() { return renderer; };
	MapCell getCasilla(int x, int y) { return map->getCell(x, y); };
	void setPower(bool cambio) { power = cambio; };
	bool getPower() { return power; };
	int getCont() { return contTiempo; };
};

