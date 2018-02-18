#include "GameMap.h"
#include "Texture.h"
#include "Game.h"

GameMap::GameMap()
{
}

GameMap::GameMap(Texture* tMuro, Texture* tComida, Texture* tVitamina, Game* game)
{
	muro = tMuro;
	comida = tComida;
	vitamina = tVitamina;
	texColVit = 0;
}

GameMap::~GameMap()
{
	if (matrizJuego != nullptr)
	{
		for (int i = 0; i < cols; i++)
			delete[] matrizJuego[i];
		delete[] matrizJuego;
	}
}

//1. METODOS PARA EL FUNCIONAMIENTO BASICO 
//1.1 Metodo para renderizar el mapa (sin personajes)
void GameMap::render(SDL_Renderer* renderer, Game* game)
{
	SDL_Rect destRect;
	destRect.x = destRect.y = 0;
	destRect.w = game->getFrameW();
	destRect.h = game->getFrameH();

	for (int i = 0; i < fils; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			destRect.x = j * destRect.w;
			destRect.y = i * destRect.h;
			if (matrizJuego[i][j] == Wall)
				muro->render(renderer, destRect);
			else if (matrizJuego[i][j] == Food)
				comida->render(renderer, destRect);
			else if (matrizJuego[i][j] == Vitamins)
				vitamina->renderFrame(renderer, destRect, 0, texColVit);
		}
	}

	if (texColVit == 0)
		texColVit = 1;
	else
		texColVit = 0;
}

//2. METODOS PARA CREAR EL MAPA
//2.1 Metodo para inicializar la matriz dinamica del tablero
void GameMap::creaMatrizVacia(int x, int y)
{
	fils = x; cols = y;
	matrizJuego = new MapCell*[fils];
	for (int i = 0; i < fils; i++)
	{
		matrizJuego[i] = new MapCell[cols];
	}
}

//2.2 Metodo para rellenar la matriz del juego
void GameMap::creaMapa(int n, int i, int j, Game* game)
{
	if (n == 1) matrizJuego[i][j] = Wall;
	else if (n == 2)
	{
		matrizJuego[i][j] = Food;
		game->setNumComida(1);
	}
	else if (n == 3)
	{
		matrizJuego[i][j] = Vitamins;
		game->setNumComida(1);
	}
	else matrizJuego[i][j] = Empty;
}

//3. METODOS PARA COMPROBAR UNA POSICION DETERMINADA
//3.1 Consulta una posicion especifica para saber si el personaje se puede mover o no, tambien pasa un numero que indica que tipo de celda es
bool GameMap::consultaPos(int x, int y)
{
	if (matrizJuego[x][y] == Wall)
	{
		return false;
	}
	else
		return true;
}

//3.2 Metodo que cambia la posicion indicada a vacia
void GameMap::escribePos(int x, int y, Game* game)
{
	if (matrizJuego[x][y] == Vitamins) 
		game->setPower(true);

	matrizJuego[x][y] = Empty;
}
