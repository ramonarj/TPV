//Dependencias
#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>


using namespace std;
void Game::InitSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int winX, winY;
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man 2 by Ale & Ramon", winX, winY,
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
	string filename = hud.chooseGame();

	//2.Bucle del juego
	while (!exit && !win && !error && !gameover)
	{
		//2.1.Cargamos el mapa y lo renderizamos con las letras de "ready"
		loadMap(filename);
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
			if (saveState)
				SaveState();
		}
		
		SDL_Delay(READY_DELAY);
		//2.3.Paso de nivel
		if (win && level < MAX_LEVEL)
		{
			deleteGhosts();
			level++;
			filename = "level0" + to_string(level) + ".pac";
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
			if (event.key.keysym.sym == SDLK_UP)
				pac->setDir(0, -1);
			else if (event.key.keysym.sym == SDLK_LEFT)
				pac->setDir(-1, 0);
			else if (event.key.keysym.sym == SDLK_DOWN)
				pac->setDir(0, 1);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				pac->setDir(1, 0);
			else if (event.key.keysym.sym == SDLK_s)
				saveState = true;
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit = true;
		}
	}
}

void Game::Update()
{
	for (GameCharacter* c : characters)
		c->update();
}

void const Game::Render()
{
	//Clear
	SDL_RenderClear(renderer);

	//Cada objeto del juego se pinta
	map->render(); //Mapa   
	for (GameCharacter* c : characters) //Personajes
		c->render();
	hud.render();  

	//Pintamos todo
	SDL_RenderPresent(renderer);
}

bool Game::nextCell(int x, int y)
{
	//No hay muro
	if (map->getCell(y,x) != Wall)
		return true;
	else
		return false;
}

bool Game::eatFood(int x, int y)
{
	bool vitamin = false;
	if (map->getCell(y, x) != Empty)
	{
		if (map->getCell(y, x) == Vitamins)
			vitamin = true;
		//Se come la comida
		map->eatFood(y, x);
		addScore(10);
	}
	return vitamin;
}
void Game::killPacman()
{
	pac->die();
	//Para que quede bonito
	if (!gameover)
	{
		list<GameCharacter*>::iterator it = characters.begin();
		it++;
		while (it != characters.end())
		{
			(*it)->die();
			it++;
		}
		Render();
		hud.renderReady();
		SDL_Delay(READY_DELAY);
	}
}

void Game::deleteGhosts()
{
	list<GameCharacter*>::iterator it = characters.begin();
	it++;

	while (it != characters.end())
	{
		delete (*it);
		it = characters.erase(it);
	}
}

void Game::loadMap(string filename)
{
	bool archivoGuardado = (filename[0] != 'l'); //Para las partidas guardadas

	filename = LEVELS_PATH + filename;
	ifstream archivo;
	
	archivo.open(filename);
	if (archivo.is_open())
	{
		//Si era una partida guardada, hay que leer el nivel y puntuación
		if (archivoGuardado)
			archivo >> level >> score;

		delete map;
		map = new GameMap(textures[WallText], textures[FoodText], textures[VitaminText], this);
		map->loadFromFile(archivo);

		//Inicializamos el tamaño de celda
		cellHeight = winHeight / map->getSize(0);
		cellWidth = (winWidth / map->getSize(1)) * 4 / 5;
		
		unsigned int num_ghosts;
		archivo >> num_ghosts;
		unsigned int i = 0; unsigned int ghostNo = 0;
		//Creamos los fantasmas
		while (i < num_ghosts && !archivo.fail())
		{
			Ghost* ghost;
			int ghostType;
			archivo >> ghostType;
			if (ghostType == 0)
			{
				ghost = new Ghost(textures[CharactersText], this, ghostNo);
				ghostNo++;
			}
				
			else
				ghost = new SmartGhost(textures[CharactersText], this);
			ghost->loadFromFile(archivo);
			characters.push_back(ghost);
			i++;
		}
		//En todos los niveles menos el 1º
		if (pac == nullptr)
		{
			pac = new Pacman(textures[CharactersText], this);
			characters.insert(characters.begin(), pac);
		}
		   
		pac->loadFromFile(archivo);
	}
	else
		error = true;

	archivo.close();
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

bool Game::hayFantasma(int x, int y, list<GameCharacter*>::iterator & it)
{
	it = characters.begin();
	it++; //Para no contar a PacMan
	bool fantasma = false;
	while (it != characters.end() && !fantasma)
	{
		fantasma = (*it)->getPosX() == x && (*it)->getPosY() == y;
		if (!fantasma)
			it++;
	}
	return fantasma;
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
			while (i<10)
			{
				archivo << bestScores[i] << endl;
				i++;
			}
		}
		archivo.close();
	}
}

void Game::killGhost(list<GameCharacter*>::iterator it)
{
	(*it)->die(); 
	addScore(200);

	//Si era un smartGhost muerto, lo eliminamos de la lista y lo borramos
	SmartGhost* smartGhost = dynamic_cast<SmartGhost*>(*it);
	if (smartGhost != nullptr && smartGhost->isDead())
	{
		characters.erase(it);
		delete smartGhost;
	}
}

void Game::SaveState()
{
	hud.renderPausa();
	SDL_Event event;
	int code = 0, i = 0;

	while (saveState && !exit)
	{
		//Esperamos al evento
		SDL_WaitEvent(&event);

		//Sale del juego con ratón
		if (event.type == SDL_QUIT)
			exit = true;
		
		else if (event.type == SDL_KEYDOWN)
		{
			//Añade un código
			if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
			{
				code = code * 10 + (event.key.keysym.sym - SDLK_0);
				i++;
				hud.renderCode(event.key.keysym.sym - SDLK_0);
			}

			//Sale con Escape
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit = true;
			//Guarda el código
			else if (event.key.keysym.sym == SDLK_RETURN)
			{
				saveState = false;
				SaveGame(code);
			}	
		}
		else if (i > 8)
		{
			saveState = false;
			SaveGame(code);
		}
	}
}

void Game::SaveGame(int code)
{
	ofstream file;
	file.open(LEVELS_PATH + to_string(code) + ".pac");
	if (file.is_open())
	{
		//Nivel y puntuación
		file << level << " " << score << endl;
		//Mapa
		map->saveToFile(file);
		//Personajes
		file << characters.size() - 1 << endl;
		list<GameCharacter*>::reverse_iterator rit = characters.rbegin();
		while (rit != characters.rend() && !file.fail())
		{
			if (typeid (*(*rit)) == typeid (Ghost))
				file << "0 ";
			else if (typeid (*(*rit)) == typeid (SmartGhost))
				file << "1 ";
			(*rit)->saveToFile(file);
			file << endl;
			rit++;
		}
	}
	file.close();
}

void Game::createBabyGhost(unsigned int x, unsigned int y)
{
	SmartGhost* baby = new SmartGhost(textures[CharactersText], this, x, y);
	characters.push_back(baby);
}

Game::~Game()
{
	//Destruimos los personajes
	for (GameCharacter*c : characters)
		delete c;
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