//Dependencias
#include "Texture.h"
#include <iostream>
#include "Exceptions.h"

Texture::Texture()
//Secuencia de inicialización
	:w(0), h(0), fw(0), fh(0), numRows(0), numCols(0), texture(nullptr){}

//Carga una textura de un archivo. Se pueden indicar el nº de filas y columnas de la textura
void Texture::load(SDL_Renderer* renderer, string filename,
	unsigned int numRows, unsigned int numCols)
{
	//Creamos la superficie de píxeles
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface != nullptr)
	{
		//Hacemos la textura y liberamos la superficie
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (texture == nullptr)
			throw SDLError(IMG_GetError());
		//Obtenemos las dimensiones
		SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

		//Para los frames
		this->numRows = numRows;
		this->numCols = numCols;
		fw = w / numCols;
		fh = h / numRows;
	}
	else
		throw SDLError(IMG_GetError());
}

//Renderiza la textura completa en un determinado rectángulo
void const Texture::render(SDL_Renderer* renderer, const SDL_Rect& rect,
	SDL_RendererFlip flip)
{
	//Solamente hacemos RenderCopy
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

//Renderiza un frame determinado de la textura
void const Texture::renderFrame(SDL_Renderer* renderer, const SDL_Rect& destRect,
	int row, int col, SDL_RendererFlip flip)
{
	//Cogemos el frame de origen
	SDL_Rect srcRect = { fw * col, fh * row, fw, fh};

	//Hacemos RenderCopy
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

Texture::~Texture()
{

}
