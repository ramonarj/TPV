#include "Texture.h"
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

using namespace std;


Texture::Texture()
{
}


Texture::~Texture()
{


}

bool Texture::load(SDL_Renderer* renderer, string filename,
	unsigned int rows, unsigned int cols)
{
	numRows = rows;
	numCols = cols;

	//Creamos la textura a partir de la surface
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface != nullptr)
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture == nullptr)
			cout << "Fallo en la carga de la imagen";

		//Obtenemos las dimensiones
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
		fw = w / numCols;
		fh = h / numRows;

		srcRect.w = fw;
		srcRect.h = fh;

		//Liberamos la superficie
		SDL_FreeSurface(surface);
		return true;
	}
	else
		return false;
}

void Texture::render(SDL_Renderer* renderer, const SDL_Rect& rect, SDL_RendererFlip flip)
{
	//Hacemos el rendercopy
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Texture::renderFrame(SDL_Renderer* renderer, const SDL_Rect& destRect, int row, int col, SDL_RendererFlip flip)
{
	//Elige el frame de origen
	srcRect.x = fw * col;
	srcRect.y = fh * row;

	//Lo pinta
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
