//Dependencias
#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
#include "Exceptions.h"


using namespace std;
Game::Game()
{
	//1.Inicializamos SDL y la semilla de random
	try
	{
		InitSDL();
		srand(unsigned int(time(nullptr)));
		//2.Cargamos las texturas
		for (unsigned int i = 0; i < NUM_TEXTURES; i++)
		{
			try
			{
				textures[i] = new Texture();
				const TextureAtributes& atributes = TEXTURES_ATRIBUTES[i];
				textures[i]->load(renderer, IMAGES_PATH + atributes.filename,
					atributes.numRows, atributes.numCols);
			}
			catch (SDLError& e)
			{
				cout << e.what() << endl;
			}
		}
		//3.Cargamos las mejores puntuaciones y el HUD
		hud = new HUD(this);
		loadScores();

		//4.Máquina de estados
		stateMachine = new GameStateMachine();
		stateMachine->pushState(new MainMenuState(this, hud));

	}
	catch (SDLError& e)
	{
		cout << e.what() << endl;
	}
}

//METODOS PRIVADOS
void Game::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw SDLError(SDL_GetError());
	else
	{
		unsigned int winX, winY;
		winX = winY = SDL_WINDOWPOS_CENTERED;
		window = SDL_CreateWindow("Pac-Man 2 by Ale & Ramon", winX, winY,
			winWidth, winHeight, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		//Vemos si hay algún error
		if (window == nullptr || renderer == nullptr)
			throw SDLError(SDL_GetError());
		else
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) && !exit)
	{
		if (event.type == SDL_QUIT)
			exitGame();
		else
			stateMachine->currentState()->handleEvent(event);
	}
}

void Game::Update()
{
	//Llamamos al update del estado actual
	stateMachine->currentState()->update();
}

void const Game::Render()
{
	//Clear
	SDL_RenderClear(renderer);

	//LLamamos al estado actual
	stateMachine->currentState()->render();

	//Pintamos todo
	SDL_RenderPresent(renderer);
}



//METODOS PUBLICOS
void Game::Run()
{
	unsigned int startTime, frameTime;
	while (!exit)
	{
		startTime = SDL_GetTicks();
		handleEvents();
		Update();
		Render();
		frameTime = SDL_GetTicks() - startTime;
		if (frameTime < FRAME_RATE)
			SDL_Delay(FRAME_RATE - frameTime);
	}
}

void Game::loadScores()
{
	ifstream archivo;
	archivo.open("scores");

	//Vemos si existe el archivo
	if (archivo.is_open())
	{
		int i = 0;
		//Leemos hasta que se acabe
		while (!archivo.fail())
		{
			archivo >> bestScores[i];
			//Pasamos de línea
			string s;
			getline(archivo, s);
			i++;
		}
	}
	archivo.close();
}

void const Game::saveScore(int score)
{
	//Miramos si podemos introducir la nueva puntuación
	if (score > bestScores[9])
	{
		//1.Ponemos la nueva puntuación
		//Encontramos donde ponerla
		unsigned int j = 0;
		while (score < bestScores[j])
			j++;
		//Corremos la lista
		for (unsigned int i = 9; i > j; i--)
			bestScores[i] = bestScores[i - 1];
		//Insertamos la nueva puntuación
		bestScores[j] = score;

		//2.Guardamos las puntuaciones
		ofstream archivo;
		archivo.open("scores");
		if (archivo.is_open())
		{
			unsigned int i = 0;
			//Vamos esctibiendo las puntuaciones
			while (i<10)
			{
				archivo << bestScores[i] << endl;
				i++;
			}
		}
		archivo.close();
	}
}

int Game::enterCode()
{
	SDL_Event event;
	int code = 0, i = 0;
	bool saveState = true;

	while (saveState)
	{
		//Esperamos al evento
		SDL_WaitEvent(&event);

		if (event.type == SDL_KEYDOWN)
		{
			//Añade un código
			if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
			{
				code = code * 10 + (event.key.keysym.sym - SDLK_0);
				i++;
				hud->renderCode(event.key.keysym.sym - SDLK_0);
			}
			//Guarda el código
			else if (event.key.keysym.sym == SDLK_RETURN)
				saveState = false;

		}
		else if (i > 8)
			saveState = false;
	}
	return code;
}

bool Game::alterna(int&valorComparado, int x, int y)
{
	if (valorComparado == x)
		valorComparado = y;
	else if (valorComparado == y)
		valorComparado = x;
	else
		return false;
	return true;
}

bool Game::ParesOpuestos(Par par1, Par par2)
{
	if (par1.x == -par2.x && par1.y == -par2.y)
		return true;
	else
		return false;

}


Game::~Game()
{
	//Destruimos la máquina de estados
	delete stateMachine;
	//Destruimos el hud
	delete hud;
	//Destruimos las texturas
	for (unsigned int i = 0; i < NUM_TEXTURES; i++)
		delete textures[i];
	//Cerramos SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}