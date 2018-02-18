//Dependencias
#include "HUD.h"
#include "Game.h"
#include <iostream>
#include "Texture.h"

using namespace std;

HUD::HUD() 
//Secuencia de inicialización
:posX(0), posY(0), width(0), height(0), scoreFrame(0), juego(nullptr)
{
	//Texturas
	for (int i = 0; i < HUD_TEXTURES; i++)
	{
		textures[i] = nullptr;
		rect[i] = { 0, 0, 0, 0 };
	}
		
}

HUD::HUD(unsigned int width, unsigned int height, Texture* textures[], Game* juego)
//Secuencia de inicialización
:posX(juego->getWindowSize(0) - width), posY(0), width(width), height(height), scoreFrame(0),
juego(juego)
{
	//Inicializamos las texturas
	for (int i = 0; i < HUD_TEXTURES; i++)
	{
		this->textures[i] = textures[i];
		rect[i] = { 0, 0, 0, 0 };
	}
		
	//Creamos los rectángulos para el renderizado
	rect[LivesText] = { posX, (int)(height -(width / 3)), (int)(width / 5), (int)(width / 5) };
	rect[ScoreText] = { posX, (int)(height / 4), (int)(width / 3), (int)(height / 25) };
	rect[HighScoreText] = { posX, (int)(height / 15), (int)(width), (int)(height / 25) };
	rect[NumbersText] = { posX + (int)(7 * width / 10), (int)(height / 3), (int)(width / 7), (int)(height / 25) }; 
	rect[WinText] =  { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 15) };
	rect[GameOverText] = { posX - (int)(width / 10), (int)(height / 2), (int)(width), (int)(height / 15) };
	rect[ReadyText] = { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 20) };
	rect[MenuText] = { 0, 0, juego->getWindowSize(0), juego->getWindowSize(1) };
}	

void HUD::render()
{
	//1.Vidas
	int x = rect[LivesText].x;
	for (unsigned int i = 0; i < juego->livesLeft(); i++)
	{
		//Pintamos y nos movemos a la derecha
		textures[LivesText]->render(juego->getRenderer(), rect[LivesText]);
		rect[LivesText].x += (rect[LivesText].w + 5);
	}
	//Volvemos a ponerlo en su sitio
	rect[LivesText].x = x;
	
	//2.Puntuación (letras): parpadean
	if (scoreFrame < 3)
	{
		textures[ScoreText]->render(juego->getRenderer(), rect[ScoreText]);
		scoreFrame++;
	}
	else
		scoreFrame = 0;

	//3.Puntuación (números)
	int scoreNo = juego->getScore();
	renderScore(scoreNo);

	//4.High Score (letras)
	textures[HighScoreText]->render(juego->getRenderer(), rect[HighScoreText]);

	//5.High Score (números)
	if (juego->highScore() > scoreNo)
		scoreNo = juego->highScore();
	rect[NumbersText].y -= (int)(rect[NumbersText].h * 5);
	renderScore(scoreNo);
	rect[NumbersText].y += (int)(rect[NumbersText].h * 5);
}

void HUD::renderMenu()
{
	//6.Menu
	textures[MenuText]->render(juego->getRenderer(), rect[MenuText]);
	SDL_RenderPresent(juego->getRenderer());
}

void HUD::renderEspecial(bool winGame)
{
	//7.Win
	if (winGame)
		textures[WinText]->render(juego->getRenderer(), rect[WinText]);
	//8.GameOver
	else
		textures[GameOverText]->render(juego->getRenderer(), rect[GameOverText]);
	SDL_RenderPresent(juego->getRenderer());
}

void HUD::renderReady()
{
	//9.Ready
	textures[ReadyText]->render(juego->getRenderer(), rect[ReadyText]);
	SDL_RenderPresent(juego->getRenderer());
}

void HUD::renderScore(int number)
{
	int x = rect[NumbersText].x;
	while (number > 0)
	{
		//Pintamos el número
		int i = number % 10;
		textures[NumbersText]->renderFrame(juego->getRenderer(), rect[NumbersText], 0, i);
		number /= 10;
		//Nos movemos a la izda.
		rect[NumbersText].x -= rect[NumbersText].w;
	}
	//Volvemos a 'ponerlo en su sitio
	rect[NumbersText].x = x;
}

HUD::~HUD()
{

}
