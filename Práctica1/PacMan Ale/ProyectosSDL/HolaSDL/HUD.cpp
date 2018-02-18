#include "HUD.h"
#include "Game.h"

HUD::HUD()
{
}

HUD::HUD(Texture* tVida, Texture* tMenu, Texture* tReady, Texture* tGameOver, Texture* tWin, Texture* tPuntuacion, Texture* tPuntos, Game* game)
{
	juego = game;
	textures[ReadyText] = tReady;
	textures[FondoMenuText] = tMenu;
	textures[GameOverText] = tGameOver;
	textures[WinText] = tWin;
	textures[PuntuacionText] = tPuntuacion;
	for (int i = 0; i < NUM_TEXTURES_PUNTOS; i++)
		textPuntos[i] = tPuntos;
	for (int i = 0; i < NUM_TEXTURES_VIDAS; i++)
		textVidas[i] = tVida;
}

HUD::~HUD()
{
}

//1. METODOS PARA RENDERIZAR LAS TEXTURAS DEL HUD
//1.1 Metodo para renderizar los mensajes que aparecen en la parte inferior de la pantalla: READY!, WIN, GAMEOVER
void HUD::renderMensaje(SDL_Renderer* renderer, bool gameOver, bool win)
{
	SDL_Rect destRect;
	destRect.w = WIN_WIDTH / 7;
	destRect.h = (WIN_HEIGHT - (WIN_HEIGHT / 11)) / 18;
	destRect.x = (WIN_WIDTH / 2) - (destRect.w / 2);
	destRect.y = (WIN_HEIGHT - (WIN_HEIGHT / 22)) - (destRect.h / 2);

	if (!gameOver && !win)
		textures[ReadyText]->render(renderer, destRect);
	else if (gameOver && !win)
		textures[GameOverText]->render(renderer, destRect);
	else if (!gameOver && win)
		textures[WinText]->render(renderer, destRect);
}

//Metodo para renderizar la pantalla inicial
void HUD::renderMenu(SDL_Renderer* renderer)
{
	SDL_Rect destRect;
	destRect.w = WIN_WIDTH;
	destRect.h = WIN_HEIGHT;
	destRect.x = destRect.y = 0;

	textures[FondoMenuText]->render(renderer, destRect);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
}

//Metodo para renderizar las vidas restantes
void HUD::renderVidas(SDL_Renderer* renderer, int numVidas)
{
	SDL_Rect destRect;
	destRect.w = juego->getFrameW();
	destRect.h = juego->getFrameH();
	destRect.y = (juego->getFilsTab() + 0.5) * juego->getFrameH();
	for (int i = 0; i < numVidas; i++)
	{
		destRect.x = (i + 1) * juego->getFrameW();
		textVidas[i]->renderFrame(renderer, destRect, 2, 11);
	}
}

//Metodo para renderizar las puntuaciones
void HUD::renderPuntuacion(SDL_Renderer* renderer)
{
	SDL_Rect destRect;
	destRect.w = juego->getFrameW() * 4;
	destRect.h = juego->getFrameH();
	destRect.x = juego->getFrameW();
	destRect.y = (juego->getFilsTab() + 2) * juego->getFrameH();

	textures[PuntuacionText]->render(renderer, destRect);
}

//Metodo para controlar los numeros de la puntuacion
void HUD::renderPuntos(SDL_Renderer* renderer, int puntos)
{
	int x = 20;
	SDL_Rect destRect;
	destRect.w = juego->getFrameW();;
	destRect.h = juego->getFrameH();
	destRect.x = WIN_WIDTH / 2;
	destRect.y = (juego->getFilsTab() + 2) * juego->getFrameH();

	//textures[PuntosText]->renderFrame(renderer, destRect, 0, 0);
	/*
	for (int i = 0; i < 10; i++)
	{
		textures[PuntosText]->renderFrame(renderer, destRect, 0, 0);
	}*/
}
