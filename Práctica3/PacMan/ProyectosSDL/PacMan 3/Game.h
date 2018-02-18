//Dependencias
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <time.h>
#include <string>
#include "Ghost.h"
#include "SmartGhost.h"
#include "Pacman.h"
#include "GameMap.h"
#include "HUD.h"
#include "checkML.h"
#include <list>
#include "GameStateMachine.h"
#include "MainMenuState.h"

using namespace std;

//Struct para los atributos de fantasma y enumerado para las texturas
struct TextureAtributes
{
	string filename;
	int numRows;
	int numCols;
};
enum TextureName {
	LivesText, ScoreText, MenuText, WinText, GameOverText,
	ReadyText, HighScoreText, NumbersText, NewGameText, LoadGameText,
	ScoreMenuText, BackMenuText, PauseText, ExitText, ReanudarText, SaveText,
	CharactersText, WallText, FoodText, VitaminText,
};

//Constantes
const string IMAGES_PATH = "..\\images\\";
const string LEVELS_PATH = "..\\levels\\";
const unsigned int FRAME_RATE = 140;
const unsigned int NUM_TEXTURES = 20;
const unsigned int MAX_LEVEL = 2;
const unsigned int READY_DELAY = 1000;
const unsigned int NUM_SCORES = 10;

//Atributos de cada textura (nombre y nº de filas y columnas)
const TextureAtributes TEXTURES_ATRIBUTES[NUM_TEXTURES] = { { "Vida.png", 1, 1 },
{ "score.png", 1, 1 },{ "menu.png", 1, 1 },{ "win.png", 1, 1 },
{ "gameover.png", 1, 1 },{ "ready.png", 1, 1 },{ "highscore.png", 1, 1 },{ "numbers.png", 1, 10 },
{ "nuevaPartida.png", 1, 1 },{ "cargarPartida.png", 1, 1 }, { "menuScore.png", 1, 1 },
{ "menuButton.png", 1, 1 }, { "pause.png", 1, 1 }, { "exitButton.png", 1, 1}, { "reanudarButton.png", 1, 1 }, { "saveButton.png", 1, 1 },
{ "characters1.png", 4, 14 },{ "wall2.png", 1, 1 },{ "food2.png", 1, 1 },{ "vitamin.png", 1, 2 } 
};


//Definición de la clase
class Game
{
	//Atributos y métodos privados
private:
	//Para SDL
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	const unsigned int winWidth = 800;
	const unsigned int winHeight = 600;

	//Arrays de objetos y de punteros a texturas
	GameStateMachine* stateMachine;
	HUD* hud;

	//Punteros a las texturas
	Texture* textures[NUM_TEXTURES];

	//Booleanos de control
	bool exit = false;
	int bestScores[NUM_SCORES];	

	//Inicializa SDL
	void InitSDL();

	//Métodos que se llaman cada tick
	void handleEvents();
	void Update();
	void const Render();

	//Carga las mejores puntuaciones
	void loadScores();


public:
	//Constructora por defecto
	Game();

	//Bucle del juego
	void Run();

	//GETTERS
	//Devuelve el renderer
	SDL_Renderer* getRenderer(){ return renderer; };

	//Devuelven las dimensiones de las celdas (0=Width, 1=Height)
	int getWindowSize(unsigned int i){ if (i == 0) return winWidth; else return winHeight; };
	
	//Devuelve una textura
	Texture* getTexture(int i){ return textures[i]; };
	
	//Nos dice el HighScore
	int highScore(){ return bestScores[0]; };
	
	//Nos dice la mejor puntuacion
	int bestScore(int i){ return bestScores[i]; };
	
	//Devuelven la stateMachine y el hud
	GameStateMachine* getStateMachine() { return stateMachine; };
	HUD* getHud() { return hud; };

	//Otros
	//Guarda la puntuación actual en el archivo
	void const saveScore(int score);

	//Sale del juego
	void exitGame(){ exit = true; };

	//Introduce un código
	int enterCode();



	//ESTÁTICOS
	//Alterna entre dos valores dados
	static bool alterna(int& valorComparado, int x, int y);

	//Determina si los pares son opuestos (izda es opuesto a dcha)
	static bool ParesOpuestos(Par par1, Par par2);
	

	
	//Destructora
	~Game();
};
