#pragma once

#include "SDL.h"
#include <string>

using namespace std;

class Texture
{
private:
	SDL_Texture* texture = nullptr;
	//Altura y anchura de la textura
	int w = 0;
	int h = 0;
	//Altura y anchura del frame
	int fw = 0;
	int fh = 0;
	//Numero de filas y columnas de la textura
	int numRows = 1;
	int numCols = 1;

	SDL_Rect srcRect;

public:
	Texture();
	~Texture();

	//Para cargar la textura
	bool load(SDL_Renderer* renderer, string filename,
		unsigned int numRows = 1, unsigned int numCols = 1);

	//Para renderizarla
	void render(SDL_Renderer* renderer, const SDL_Rect& rect,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Para renderizar un frame
	void renderFrame(SDL_Renderer* renderer, const SDL_Rect& destRect,
		int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

