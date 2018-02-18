//Dependencias
#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "checkML.h"

using namespace std;

//Definición de la clase
class Texture
{
private:
	//Puntero a la textura de SDL
	SDL_Texture* texture;
	//Altura y anchura de la textura
	int w;
	int h;
	//Altura y anchura del frame
	int fw;
	int fh;
	//Número de filas y columnas de la textura
	int numRows, numCols;
	

public:
	Texture();
	void load(SDL_Renderer* renderer, string filename,
		unsigned int numRows = 1, unsigned int numCols = 1);
	void const render(SDL_Renderer* renderer, const SDL_Rect& rect,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	void const renderFrame(SDL_Renderer* renderer, const SDL_Rect& destRect,
		int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);
	~Texture();
};
