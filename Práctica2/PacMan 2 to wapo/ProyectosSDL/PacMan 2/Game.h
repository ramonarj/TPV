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
	ReadyText, HighScoreText, NumbersText, NewGameText, LoadGameText, PuntuacionesText,
	ScoreMenuText, VolverText, PauseText, CharactersText, WallText, FoodText, VitaminText
};

//Constantes
const string IMAGES_PATH = "..\\images\\";
const string LEVELS_PATH = "..\\levels\\";
const unsigned int FRAME_RATE = 140;
const unsigned int NUM_TEXTURES = 18;
const unsigned int MAX_LEVEL = 2;
const unsigned int READY_DELAY = 1000;
const unsigned int NUM_SCORES = 10;

//Atributos de cada textura (nombre y nº de filas y columnas)
const TextureAtributes TEXTURES_ATRIBUTES[NUM_TEXTURES] = { { "Vida.png", 1, 1 },
{ "score.png", 1, 1 },{ "menu.png", 1, 1 },{ "win.png", 1, 1 },
{ "gameover.png", 1, 1 },{ "ready.png", 1, 1 },{ "highscore.png", 1, 1 },{ "numbers.png", 1, 10 },
{ "nuevaPartida.png", 1, 1 },{ "cargarPartida.png", 1, 1 },{ "puntuaciones.png",1, 1 },{ "menuScore.png", 1, 1 },
{ "volver.png", 1, 1 }, { "pause.png", 1, 1 },  { "characters1.png", 4, 14 },{ "wall2.png", 1, 1 },{ "food2.png", 1, 1 },{ "vitamin.png", 1, 2 } };


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
	int cellWidth, cellHeight;

	//Arrays de objetos y de punteros a texturas
	list<GameCharacter*> characters;
	Pacman* pac = nullptr; //Puntero adicional a PacMan
	HUD hud;
	GameMap* map = nullptr;

	//Punteros a las texturas
	Texture* textures[NUM_TEXTURES];

	//Booleanos de control
	bool exit, error, gameover, win, saveState = false;

	//Para extender la partida
	int level = 1;
	unsigned int score = 0;
	unsigned int bestScores[NUM_SCORES];	

	//Métodos privados
	//Inicializa SDL
	void InitSDL();

	//Métodos que se llaman cada tick
	void handleEvents();
	void Update();
	void const Render();

	//Lee el mapa
	void loadMap(string filename);
	//Suma puntos a la puntuación actual
	void addScore(unsigned int i);
	//Guarda la puntuación actual en el archivo
	void const saveScore();
	//Carga las mejores puntuaciones
	void loadScores();
	//Borra a los personajes para pasar de nivel
	void deleteGhosts();
	//Estados del juego (aparte del normal)
	void SaveState();
	//Guarda la partida actual
	void SaveGame(int code);


	//Métodos públicos
public:
	//Constructora por defecto
	Game();
	//Bucle del juego
	void Run();
	//Pregunta por una celda y cambia las posiciones en caso de que se salgan del mapa
	bool nextCell(int x, int y);
	//Devuelve el renderer del juego
	SDL_Renderer* getRenderer(){ return renderer; };
	//Devuelven las dimensiones de las celdas (0=Width, 1=Height)
	int getCellSize(unsigned int i){ if (i == 0) return cellWidth; else return cellHeight; };
	//Devuelven las dimensiones de las celdas (0=Width, 1=Height)
	int getWindowSize(unsigned int i){ if (i == 0) return winWidth; else return winHeight; };


	//Indica si hay fantasma o no en esa posición
	bool hayFantasma(int x, int y, list<GameCharacter*>::iterator & it);
	//"Termina" la partida
	void setgameOver(){ gameover = true; };
	//"Termina" la partida
	bool getgameOver(){ return gameover; };
	//Dice si el power-up está activo o no
	bool const powerUpActive(){ return pac->powerUpActive(); };
	unsigned int const timeLeft(){ return pac->timeLeft(); };
    //Nos dice las vidas restantes
	unsigned int const livesLeft(){ return pac->livesLeft(); };
	//Nos dice la puntuación actual
	unsigned int const getScore(){ return score; };
	//Nos dice el HighScore
	int highScore(){ return bestScores[0]; };
	//Comprueba que x e y están en los límites del mapa, recolocándolos si es necesario
	void const compruebaLimites(int &x, int &y);
	//Pacman se come la comida de una posición
	bool eatFood(int x, int y);
	//Mata a un fantasma
	void killGhost(list<GameCharacter*>::iterator it);
	//Mata a Pacman
	void killPacman();
	//Completa el nivel
	void Win(){ win = true; };
	//Devuelve la posición de Pacman(para los smartGhosts)
	int getPacPos(int i){ if (i == 0) return pac->getPosX(); else return pac->getPosY(); };
	//Crea un bebé smartGhost en la posición dada
	void createBabyGhost(unsigned int x, unsigned int y);


	//Estáticos
	//Alterna entre dos valores dados
	static bool alterna(int& valorComparado, int x, int y);
	//Determina si los pares son opuestos (izda es opuesto a dcha)
	static bool ParesOpuestos(Par par1, Par par2);
	//Destructora
	~Game();
};
