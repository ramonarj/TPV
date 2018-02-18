//Dependencias
#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
void Game::InitSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int winX, winY;
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man by Ale & Ramon", winX, winY,
		winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Vemos si hay algún error
	if (window == nullptr || renderer == nullptr)
		error = true;
	else
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha

}

Game::Game()
{
	//1.Inicializamos SDL y la semilla de random
	InitSDL();
	srand(unsigned int(time(nullptr)));

	//2.Cargamos las texturas
	for (unsigned int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i] = new Texture();
		const TextureAtributes& atributes = TEXTURES_ATRIBUTES[i];
		textures[i]->load(renderer, IMAGES_PATH + atributes.filename,
			atributes.numRows, atributes.numCols);
	}
	//3.Cargamos las mejores puntuaciones
	loadScores();

	//3.Creamos el HUD
	hud = HUD(winWidth/5, winHeight, textures, this);
}

void Game::Run()
{
	//1.Renderizamos el menú
	hud.renderMenu();
	SDL_Delay(READY_DELAY*2);

	//2.Bucle del juego
	while (!exit && !win && !error && !gameover && level < MAX_LEVEL)
	{
		//2.1.Cargamos el mapa y lo renderizamos con las letras de "ready"
		LeeMapa();
		Render();
		hud.renderReady();
		SDL_Delay(READY_DELAY);
		//2.2.Bucle de cada nivel
		unsigned int startTime, frameTime;
		while (!exit && !win && !error && !gameover)
		{
			//Cogemos eventos, ejecutamos lógica, pintamos y esperamos
			startTime = SDL_GetTicks();
			handleEvents();
			Update();
			Render();
			frameTime = SDL_GetTicks() - startTime;
			if (frameTime < FRAME_RATE)
				SDL_Delay(FRAME_RATE - frameTime);
		}
		
		SDL_Delay(READY_DELAY);
		//2.3.Paso de nivel
		if (win && level < 5)
		{
			level++;
			win = false;
		}
	}
	//3.Salida del juego
	//Error
	if (error)
	{
		cout << "SDL se cerro debido a un error" << endl;
		cout << endl;
	}
	
	//Victoria/derrota
	else if (!exit)
	{
		saveScore();
		hud.renderEspecial(win);
		while (!exit)
			handleEvents();
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	//CONTROL DE EVENTOS
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit = true;
		else if (event.type == SDL_KEYDOWN)
		{
			//4 direcciones
			if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP)
				pac.setDir(0, -1);
			else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
				pac.setDir(-1, 0);
			else if (event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
				pac.setDir(0, 1);
			else if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
				pac.setDir(1, 0);
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit = true;
		}
	}
}

void Game::Update()
{
	bool colision;
	//Movemos a PacMan y comprobamos si se ha chocado
	pac.update();
	colision = checkCollisions();

	//Movemos los fantasmas
	for (unsigned int i = 0; i < NUM_GHOSTS; i++)
		fantasmas[i].update();

	// Comprobamos colisiones otra vez(por si los fantasmas lo han pillado)
	if (!colision)
		checkCollisions();
}

void const Game::Render()
{
	//Clear
	SDL_RenderClear(renderer);
	//Cada objeto del juego se pinta
	map->render(renderer, cellWidth, cellHeight); //Mapa
	pac.render();                                 //Pac-Man
	for (unsigned int i = 0; i < NUM_GHOSTS; i++) //Fantasmas
		fantasmas[i].render();
	hud.render();                                 //HUD
	//Pintamos todo
	SDL_RenderPresent(renderer);
}

bool Game::nextCell(int &x, int &y, bool pacman)
{
	//Toroide
	compruebaLimites(x, y);
	//No hay muro
	if (map->getCell(y,x) != Wall)
	{
		//Hay comida
		if (map->getCell(y,x) != Empty && pacman)
		{
			//Vemos si es vitamina
			if (map->getCell(y, x) == Vitamins)
			{
				pac.eatVitamin();
				addScore(40);
			}
			//Se come la comida
			map->setCell(y, x, Empty);
			foodLeft--;
			addScore(10);
			if (foodLeft < 1)
				win = true;
			
		}
		return true;
	}
		
	else
		return false;
}

void Game::LeeMapa()
{
	string filename = LEVELS_PATH + "level0" + to_string(level) + ".dat";
	ifstream archivo;
	archivo.open(filename);
	if (archivo.is_open())
	{
		//Creamos el mapa
		unsigned int rows, cols;
		archivo >> rows;
		archivo >> cols;
		delete map;
		map = new GameMap(rows, cols, textures[WallText], textures[FoodText], textures[VitaminText]);

		//Inicializamos el tamaño de celda
		cellHeight = winHeight / rows;
		cellWidth = (winWidth/cols)* 4 / 5; 

		//Vamos rellenándolo
		unsigned int i = 0;
		while (i < rows && !archivo.fail())
		{
			unsigned int j = 0;
			while (j < cols && !archivo.fail())
			{
				int aux;
				archivo >> aux;
				RellenaPos(i, j, aux);
				j++;
			}
			i++;
		}
	}
	else
		error = true;

	archivo.close();
}

void const Game::RellenaPos(unsigned int i, unsigned int j, unsigned int valor)
{
	//Vacío
	if (valor == 0)
		map->setCell(i, j, Empty);
	//muro
	else if (valor == 1)
		map->setCell(i, j, Wall);
	//Comida
	else if (valor == 2)
	{
		map->setCell(i, j, Food);
		foodLeft++;
	}
	//Vitamina
	else if (valor == 3)
	{
		map->setCell(i, j, Vitamins);
		foodLeft++;
	}
	//Personajes
	else if (valor > 4)
	{
		map->setCell(i, j, Empty);
		CreaPersonaje(valor - 5, j, i);
	}
}

void Game::CreaPersonaje(unsigned int i, unsigned int x, unsigned int y)
{
	if (i < 4)
		fantasmas[i] = Ghost(textures[CharactersText],this, i, x, y);
	//Comprobamos que no está creado ya
	else 
		pac = Pacman(textures[CharactersText], this, x, y);
}

void const Game::compruebaLimites(int &x, int &y)
{
	if (x >= int(map->getSize(1)))
		x = 0;
	else if (x < 0)
		x = map->getSize(1) - 1;
	else if (y >= int(map->getSize(0)))
		y = 0;
	else if (y < 0)
		y = map->getSize(0) - 1;
}

bool Game::hayFantasma(int x, int y, unsigned int &ghostNo)
{
	unsigned int i = 0;
	bool fantasma = false;
	while (i<NUM_GHOSTS && !fantasma)
	{
		fantasma = fantasmas[i].getPosX() == x && fantasmas[i].getPosY() == y;
		if (!fantasma)
			i++;
	}
	ghostNo = i;
	return fantasma;
}

bool Game::checkCollisions()
{
	unsigned int i = 0;
	//Si hay un fantasma en la misma casilla que pacman
	if (hayFantasma(pac.positionX(), pac.positionY(), i))
	{
		//Mata al fantasma...
		if (pac.powerUpActive())
		{
			fantasmas[i].die();
			addScore(200);
		}

		//...o muere él
		else
		{
			pac.die();
			//Para que quede bonito
			if (!gameover)
			{
				for (int i = 0; i < NUM_GHOSTS; i++)
					fantasmas[i].die();
				Render();
				hud.renderReady();
				SDL_Delay(READY_DELAY);
			}
		}
		return true;
	}
	else
		return false;
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

void Game::addScore(unsigned int i)
{
	score += i;
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


void const Game::saveScore()
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
			while (i<10 && bestScores[i] != 0)
			{
				archivo << bestScores[i];
				i++;
				archivo << endl;
			}
		}
		archivo.close();
	}
}


Game::~Game()
{
	//Destruimos el mapa
	delete map;
	//Destruimos las texturas
	for (unsigned int i = 0; i < NUM_TEXTURES; i++)
		delete textures[i];
	//Cerramos SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}