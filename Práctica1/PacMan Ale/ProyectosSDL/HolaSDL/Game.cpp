#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>


//Constructora del GAME donde creamos la ventana y un nuevo mapa
Game::Game()
{
	winX = winY = SDL_WINDOWPOS_CENTERED;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pacman", winX, winY, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr)
		cout << "Error inicializando SDL";
	else
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	for (int i = 0; i < NUM_TEXTURAS; i++)
	{
		texture[i] = new Texture;
		const TextureAtributes& atributes = TEXTURES_ATRIBUTES[i];
		texture[i]->load(renderer, IMAGE_PATH + atributes.filename, atributes.fils, atributes.cols);
	}
	map = new GameMap(texture[WallText], texture[FoodText], texture[VitaminText], this);
	hud = HUD(texture[CharactersText], texture[MenuText], texture[ReadyText], texture[GameOverText], texture[WinText], texture[PuntuacionText], texture[NumerosText], this);
	power = false;
	ready = true;
	gameover = false;
	contTiempo = 0;
}


Game::~Game()
{
	delete map;
	for (int i = 0; i < NUM_TEXTURAS; i++)
	{
		delete texture[i];
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//1. METODOS BASICOS PARA EL FUNCIONAMIENTO
//1.1 Es el bucle princial del juego
void Game::run()
{
	int startTime, frameTime;
	hud.renderMenu(renderer);
	cargaMapa(level);
	renderParado(1000);
	ready = false;
	while (!exit && !win && !gameover)
	{
		startTime = SDL_GetTicks();

		handleEvents();
		update();
		render();
		partidaAcabada();
		contador(POWER_TIME);

		frameTime = SDL_GetTicks() - startTime;
		if (frameTime < RENDER_TIME) SDL_Delay(RENDER_TIME - frameTime);
	}
	renderParado(1000);
	end();
}

//1.2 Metodo que renderiza todos los objetos de la pantalla
void Game::render()
{
	SDL_RenderClear(renderer); 

	map->render(renderer, this);
	pacman.render(renderer);
	for (int i = 0; i < NUM_GHOST; i++)
	{
		ghost[i].render(renderer);
	}
	if (ready) hud.renderMensaje(renderer, gameover, win);
	else if (gameover) hud.renderMensaje(renderer, gameover, win);
	else if (win) hud.renderMensaje(renderer, gameover, win);
	hud.renderVidas(renderer, pacman.getVidas());
	hud.renderPuntuacion(renderer);

	SDL_RenderPresent(renderer);
}

//1.3 Metodo que llama al update del Pacman
void Game::update()
{
	pacman.update();
	choquePersonajes(); //Comprueba despues de moverse el pacman
	for (int i = 0; i < NUM_GHOST; i++)
	{
		ghost[i].update();
	}
	choquePersonajes(); //Comprueba despues de moverse los fantasmas;
}

//1.4 Metodo que controla los eventos de teclado
bool Game::handleEvents()
{
	SDL_Event event;

	//Control de eventos
	if (SDL_PollEvent(&event) && !exit)
	{
		if (event.type == SDL_QUIT)
			return true;
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_LEFT)
				pacman.setBuff(-1, 0);
			else if (event.key.keysym.sym == SDLK_UP)
				pacman.setBuff(0, -1);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				pacman.setBuff(1, 0);
			else if (event.key.keysym.sym == SDLK_DOWN)
				pacman.setBuff(0, 1);
			return false;
		}
		else
			return false;
	}
	else
		return false;
}

//1.5 Metodo que sirve para eliminar toda la basura
void Game::end()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//1.6 Metodo para controlar el choque entre Pacman y fantasmas
void Game::choquePersonajes()
{
	bool chocado = false;
	int i = 0;
	while (i < NUM_GHOST && !chocado)
	{
		if (pacman.getPosX() == ghost[i].getPosX() && pacman.getPosY() == ghost[i].getPosY())
		{
			if (!power)
				if (pacman.getVidas() > 0)
				{
					pacman.lifes();
					for (int j = 0; j < NUM_GHOST; j++)
						ghost[j].dead();
				}
				else
					gameover = true;
			else
			{
				ghost[i].muriendoTrue();
				ghost[i].dead();
			}

			chocado = true;
		}
		else
			i++;
	}
}

//1.8 Contador de tiempo
void Game::contador(int max)
{
	if (power) contTiempo++;
	if (contTiempo >= max)
	{
		power = false;
		contTiempo = 0;
	}
}

//1.9 Para el tiempo
void Game::renderParado(int tiempo)
{
	render();
	SDL_Delay(tiempo);
}

//2. METODOS PARA CARGAR EL TABLERO (MAPA Y PERSONAJES)
//2.1 Metodo usado para leer un archivo para cargar el mapa
void Game::cargaMapa(string filename)
{
	ifstream entrada;
	entrada.open(filename);
	if (entrada.is_open())
	{
		int x, y;
		entrada >> x;
		entrada >> y;
		map->creaMatrizVacia(x, y);
		frameWidth = WIN_WIDTH / y;
		frameHeight = WIN_HEIGHT / (x + 3);

		string salto;	//Salto de linea
		getline(entrada, salto);

		int i = 0;
		while (!entrada.fail() && i < x)
		{
			int j = 0;
			while (!entrada.fail() && j < y)
			{
				int aux;
				entrada >> aux;
				map->creaMapa(aux, i, j, this);
				if (aux > 4) crearPersonajes(aux, i, j);
				j++;
			}
			getline(entrada, salto);
			i++;
		}
	}
	entrada.close();
}

//2.2 Metodo que crea personajes en la posicion leida de archivo
void Game::crearPersonajes(int aux, int i, int j)
{
	if (aux == 9)
	{
		pacman = Pacman(texture[CharactersText], i, j, this);
	}
	else
	{
		ghost[aux - 5] = Ghost(texture[CharactersText], i, j, aux - 5, this);
	}
}

//3. METODOS PARA EL MOVIMIENTO DE LOS PERSONAJES
//3.1 Metodo que comprueba la siguiente posicion para el movimiento del Pacman
bool Game::nextCell(int& nx, int& ny)
{
	toroide(nx, ny);
	return map->consultaPos(ny, nx);
}

//3.2 Metodo que llama al escribePos del GameMap
void Game::cambiaCell(int x, int y)
{
	map->escribePos(x, y, this);
}

//3.3 Metodo que hace posible el movimiento toroidal de los personajes
void Game::toroide(int& nx, int& ny)
{
	//Hacer el toroide
	if (nx >= getColsTab()) nx = 0;
	else if (nx < 0) nx = getColsTab() - 1;
	else if (ny >= getFilsTab()) ny = 0;
	else if (ny < 0) ny = getFilsTab() - 1;
}

//3.4 Metodo para comprobar que hay un fantasma en una posicion determinada
bool Game::hayFantasma(int nx, int ny)
{
	bool encontrado = false;
	int i = 0;
	while (i < NUM_GHOST && !encontrado)
	{
		if (nx == ghost[i].getPosX() && ny == ghost[i].getPosY())
			encontrado = true;
		else
			i++;
	}
	return encontrado;
}

//4. METODOS PARA CONTROLAR SI HAS GANADO O PERDIDO
//4.1 Metodo que determina cuando la partida ha acabado
void Game::partidaAcabada()
{
	if (comidaRest == 0)
		win = true;
}
