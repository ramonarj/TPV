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
		this->textures[i] = textures[i];

	//Creamos los rectángulos para el renderizado
	rect[LivesText] = { posX, (int)(height - (width / 3)), (int)(width / 5), (int)(width / 5) };
	rect[ScoreText] = { posX, (int)(height / 4), (int)(width / 3), (int)(height / 25) };
	rect[HighScoreText] = { posX, (int)(height / 15), (int)(width), (int)(height / 25) };
	rect[NumbersText] = { posX + (int)(7 * width / 10), (int)(height / 3), (int)(width / 7), (int)(height / 25) };
	rect[WinText] = { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 15) };
	rect[GameOverText] = { posX - (int)(width / 10), (int)(height / 2), (int)(width), (int)(height / 15) };
	rect[ReadyText] = { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 20) };
	rect[MenuText] = { 0, 0, juego->getWindowSize(0), juego->getWindowSize(1) };
	rect[NewGameText] = { (int)((juego->getWindowSize(0) / 3) - (juego->getWindowSize(0) / 8)), (int)(2 * juego->getWindowSize(1) / 3), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[LoadGameText] = { (int)((2 * juego->getWindowSize(0) / 3) - (juego->getWindowSize(0) / 8)), (int)(2 * juego->getWindowSize(1) / 3), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[PuntuacionesText] = { (int)((juego->getWindowSize(0) / 2) - (juego->getWindowSize(0) / 8)), (int)(4 * juego->getWindowSize(1) / 5) + 10, juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[ScoreMenuText] = { 0, 0, juego->getWindowSize(0), juego->getWindowSize(1) };
	rect[VolverText] = { 2 * (int)(juego->getWindowSize(0) / 3), (int)(4 * juego->getWindowSize(1) / 5), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[PauseText] = { posX, (int)(juego->getWindowSize(1) / 2), juego->getWindowSize(0) / 10, juego->getWindowSize(1) / 40 };
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
	rect[NumbersText] = { posX + (int)(7 * width / 10), (int)(height / 3), (int)(width / 7), (int)(height / 25) };
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
	textures[NewGameText]->render(juego->getRenderer(), rect[NewGameText]);
	textures[LoadGameText]->render(juego->getRenderer(), rect[LoadGameText]);
	textures[PuntuacionesText]->render(juego->getRenderer(), rect[PuntuacionesText]);
	SDL_RenderPresent(juego->getRenderer());
}

string HUD::chooseGame()
{
	SDL_Event event;
	unsigned int x = 0, y = 0;
	int code = 0;
	bool buttonDown = false, exit = false, writeCode = false;
	string level;
	while (!buttonDown && !exit)
	{
		SDL_WaitEvent(&event);
		//Salir
		if (event.type == SDL_QUIT)
			exit = true;
		//Ratón
		else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
		{
			x = event.button.x;
			y = event.button.y;
			//Nueva partida
			if (insideRect(x, y, rect[NewGameText]))
			{
				buttonDown = true;
				level = "level01.pac";
			}
			//Cargar partida
			else if (insideRect(x, y, rect[LoadGameText]))
			{
				writeCode = true;
				rect[NumbersText] = { (int)((2 * juego->getWindowSize(0) / 3) - (juego->getWindowSize(0) / 10)), (int)((2 * juego->getWindowSize(1) / 3) - (juego->getWindowSize(1) / 25)), (int)(width / 9), (int)(height / 30) };
			}
			//Ver puntuaciones
			else if (insideRect(x, y, rect[PuntuacionesText]))
				renderHighScores();
		}

		//Teclado
		else if (event.type == SDL_KEYDOWN)
		{
			//Metemos un digito del codigo
			if (writeCode && event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
			{
				code = code * 10 + (event.key.keysym.sym - SDLK_0);
				renderCode(code);
			}
			//Ya termina de escribirlo
			else if (writeCode && event.key.keysym.sym == SDLK_RETURN)
			{
				buttonDown = true;
				level = to_string(code) + ".pac";
			}
		}
	}
	return level;
}

void HUD::renderHighScores()
{
	SDL_Event event;
	unsigned int x = 0, y = 0;
	bool buttonDown = false, exit = false;
	textures[ScoreMenuText]->render(juego->getRenderer(), rect[ScoreMenuText]);
	textures[VolverText]->render(juego->getRenderer(), rect[VolverText]);
	SDL_RenderPresent(juego->getRenderer());
	puntuaciones("scores");

	while (!buttonDown && !exit)
	{
		SDL_WaitEvent(&event);
		//Salir
		if (event.type == SDL_QUIT)
			exit = true;
		//Ratón
		else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
		{
			x = event.button.x;
			y = event.button.y;
			//Nueva partida
			if (insideRect(x, y, rect[VolverText]))
				buttonDown = true;
		}
	}
	renderMenu();
}

void HUD::puntuaciones(string filename)
{
	int score, i = 0;
	ifstream archivo;
	archivo.open(filename);
	rect[NumbersText] = { juego->getWindowSize(0) / 2 , juego->getWindowSize(1) / 5, (int)(width / 9), (int)(height / 30) };
	//Si ya hay archivo de puntuaciones creado
	if (archivo.is_open())
	{
		while (!archivo.fail() && i<NUM_SCORES)
		{
			archivo >> score;
			renderScore(int(score));
			SDL_RenderPresent(juego->getRenderer());
			rect[NumbersText].y += (int)(rect[NumbersText].h * 2);
			i++;
		}
	}
	//Si no hay archivo de puntuaciones creado todavía
	else
	{
		for (unsigned int i = 0; i < NUM_SCORES; i++)
		{
			renderScore(0);
			SDL_RenderPresent(juego->getRenderer());
			rect[NumbersText].y += (int)(rect[NumbersText].h * 2);
		}
	}
	archivo.close();
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

void HUD::renderPausa()
{
	textures[PauseText]->render(juego->getRenderer(), rect[PauseText]);
	SDL_RenderPresent(juego->getRenderer());
	rect[NumbersText] = { posX, (int)(juego->getWindowSize(1) / 2 + rect[PauseText].h * 1.5), (int)(width / 9), (int)(height / 30) };
}

void HUD::renderCode(int code)
{
	renderScore(code);
	SDL_RenderPresent(juego->getRenderer());
	rect[NumbersText].x += rect[NumbersText].w;
}

bool HUD::insideRect(int x, int y, SDL_Rect rect)
{
	return (x > rect.x && x < (rect.x + rect.w) && y > rect.y && y < (rect.y + rect.h));
}

void HUD::renderScore(int number)
{
	//Render del 0
	if (number == 0)
		textures[NumbersText]->renderFrame(juego->getRenderer(), rect[NumbersText], 0, 0);
	else
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

}


HUD::~HUD()
{

}
