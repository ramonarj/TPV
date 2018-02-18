//Dependencias
#include "HUD.h"
#include "Game.h"
#include <iostream>
#include "Texture.h"
#include "PlayState.h"

using namespace std;

HUD::HUD() 
//Secuencia de inicialización
:GameObject(), posX(0), posY(0), width(0), height(0), scoreFrame(0)
{
	//Texturas
	for (int i = 0; i < HUD_TEXTURES; i++)
	{
		textures[i] = nullptr;
		rect[i] = { 0, 0, 0, 0 };
	}
		
}

HUD::HUD(Game* juego)
//Secuencia de inicialización
:GameObject(juego), scoreFrame(0)
{
	width = juego->getWindowSize(0) / 5;
	height = juego->getWindowSize(1);
	posX = juego->getWindowSize(0) - width;
	posY = 0;

	//Inicializamos las texturas
	for (int i = 0; i < HUD_TEXTURES; i++)
		this->textures[i] = juego->getTexture(i);
		
	//Creamos los rectángulos para el renderizado
	rect[LivesText] = { posX, (int)(height -(width / 3)), (int)(width / 5), (int)(width / 5) };
	rect[ScoreText] = { posX, (int)(height / 4), (int)(width / 3), (int)(height / 25) };
	rect[HighScoreText] = { posX, (int)(height / 15), (int)(width), (int)(height / 25) };
	rect[NumbersText] = { posX + (int)(7 * width / 10), (int)(height / 3), (int)(width / 7), (int)(height / 25) }; 
	rect[WinText] =  { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 15) };
	rect[GameOverText] = { posX - (int)(width / 10), (int)(height / 2), (int)(width), (int)(height / 15) };
	rect[ReadyText] = { posX, (int)(height / 2), (int)(2 * width / 3), (int)(height / 20) };
	rect[MenuText] = { 0, 0, juego->getWindowSize(0), juego->getWindowSize(1) };
	rect[NewGameText] = { (int)((juego->getWindowSize(0) / 3) - (juego->getWindowSize(0) / 8)), (int)(2 * juego->getWindowSize(1) / 3), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[LoadGameText] = { (int)(( 2 * juego->getWindowSize(0) / 3) - (juego->getWindowSize(0) / 8)), (int)(2 * juego->getWindowSize(1) / 3), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[ExitText] = { (int)((juego->getWindowSize(0) / 2) - (juego->getWindowSize(0) / 8)), (int)(4 * juego->getWindowSize(1) / 5) + 10, juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };
	rect[ScoreMenuText] = { 0, 0, juego->getWindowSize(0), juego->getWindowSize(1) };
	rect[BackMenuText] = { 2 * (int)(juego->getWindowSize(0) / 3), (int)(4 * juego->getWindowSize(1) / 5), juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10 };

	rect[PauseText].w = juego->getWindowSize(0) / 4;
	rect[PauseText].h = juego->getWindowSize(1) / 12;
	rect[PauseText].x = (int)((juego->getWindowSize(0) / 2) - (rect[PauseText].w / 2));
	rect[PauseText].y = (int)(juego->getWindowSize(1) / 20);
	
	rect[ReanudarText].w = juego->getWindowSize(0) / 4;
	rect[ReanudarText].h = juego->getWindowSize(1) / 10;
	rect[ReanudarText].x = (int)((juego->getWindowSize(0) / 2) - (rect[ReanudarText].w / 2));
	rect[ReanudarText].y = (int)(juego->getWindowSize(1) / 3);
	
	rect[SaveText].w = juego->getWindowSize(0) / 4;
	rect[SaveText].h = juego->getWindowSize(1) / 10;
	rect[SaveText].x = (int)((juego->getWindowSize(0) / 2) - (rect[SaveText].w / 2));
	rect[SaveText].y = (int)(juego->getWindowSize(1) / 2);

	rect[BackMenuText].w = juego->getWindowSize(0) / 4;
	rect[BackMenuText].h = juego->getWindowSize(1) / 10;
	rect[BackMenuText].x = (int)((juego->getWindowSize(0) / 2) - (rect[BackMenuText].w / 2));
	rect[BackMenuText].y = (int)(2 * juego->getWindowSize(1) / 3);
}

void HUD::render()
{

}

void HUD::update()
{

}


void HUD::buttonMenu()
{
	changeRect(ExitText, (int)((juego->getWindowSize(0) / 2) - (juego->getWindowSize(0) / 8)), (int)(4 * juego->getWindowSize(1) / 5) + 10, juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10);
}

void HUD::render(int lives, int score)
{
	//1.Vidas
	int x = rect[LivesText].x;
	for (int i = 0; i < lives; i++)
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
	int scoreNo = score;
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

void HUD::renderHighScores()
{
	juego->getTexture(ScoreMenuText)->render(juego->getRenderer(), rect[ScoreMenuText]);
	puntuaciones("scores");
}

void HUD::buttonEnd()
{
	changeRect(BackMenuText, (int)(juego->getWindowSize(0) / 20), (int)(4 * juego->getWindowSize(1) / 5) + 10, juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10);
	changeRect(ExitText, (int)(juego->getWindowSize(0) / 1.4), (int)(4 * juego->getWindowSize(1) / 5) + 10, juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10);
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
	SDL_Delay(READY_DELAY);
}

void HUD::renderReady()
{
	//9.Ready
	textures[ReadyText]->render(juego->getRenderer(), rect[ReadyText]);
	SDL_RenderPresent(juego->getRenderer());
	SDL_Delay(READY_DELAY);
}


void HUD::buttonPausa()
{
	changeRect(BackMenuText, (int)((juego->getWindowSize(0) / 2) - (rect[BackMenuText].w / 2)), (int)(2 * juego->getWindowSize(1) / 3),
		juego->getWindowSize(0) / 4, juego->getWindowSize(1) / 10);
}

void HUD::changeRect(int text, int x, int y, int w, int h)
{
	rect[text] = { x, y, w, h };
}


void HUD::renderCode(int code)
{
	renderScore(code);
	SDL_RenderPresent(juego->getRenderer());
	rect[NumbersText].x += rect[NumbersText].w;
}


void HUD::renderScore(int number)
{
	//Render del 0
	if (number==0)
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

void HUD::puntuaciones(string filename)
{
	int score, i = 0;
	ifstream archivo;
	archivo.open(filename);
	rect[NumbersText] = { juego->getWindowSize(0) / 2, juego->getWindowSize(1) / 5, (int)(juego->getWindowSize(0) / 45), (int)(juego->getWindowSize(1) / 30) };
	//Si ya hay archivo de puntuaciones creado
	if (archivo.is_open())
	{
		while (!archivo.fail() && i<NUM_SCORES)
		{
			archivo >> score;
			renderScore(int(score));
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
			rect[NumbersText].y += (int)(rect[NumbersText].h * 2);
		}
	}
	SDL_RenderPresent(juego->getRenderer());
	archivo.close();
}


HUD::~HUD()
{

}
