#include "PlayState.h"
#include "Game.h"
#include "Exceptions.h"
#include <iostream>
#include "PauseState.h"
#include "EndState.h"


PlayState::PlayState():GameState(), pac(nullptr), map(nullptr){}
PlayState::PlayState(Game* juego, HUD* hud, string filename) : GameState(juego, hud), gameover(false), win(false), map(nullptr), pac(nullptr)
{
	loadMap(filename);
	SDL_RenderClear(juego->getRenderer());
	render();
	hud->renderReady();	
}

void PlayState::render() 
{
	hud->render(pac->livesLeft(), score);
	GameState::render();
	
}

void PlayState::update()
{
	GameState::update();

	//Victoria
	if (win && level < MAX_LEVEL)
	{
		deleteGhosts();
		level++;
		filename = "level0" + to_string(level) + ".pac";
		win = false;

		//Cargamos el siguiente mapa
		loadMap(filename);
		SDL_RenderClear(juego->getRenderer());
		render();
		hud->renderReady();
	}
	//Derrota o has ganado del todo
	else if (win || gameover)
	{
		juego->saveScore(score);
		hud->renderEspecial(win);
		juego->getStateMachine()->pushState(new EndState(juego, hud));
	}
}

void PlayState::handleEvent(SDL_Event &e)
{
	//Cambiamos al estado pausa
	if (e.key.keysym.sym == SDLK_ESCAPE)
		juego->getStateMachine()->pushState(new PauseState(juego, this, hud));
	
	else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
	{
		SDL_Point point = { e.button.x, e.button.y };
		list<GameObject*>::iterator it = stage.begin();
		it++; it++;
		while (it != stage.end())
		{
			Ghost* ghost = dynamic_cast<Ghost*>(*it);
			if (ghost != nullptr)
			{
				SDL_Rect thisRect = { (ghost->getPosX() * getCellSize(0)), (ghost->getPosY() * getCellSize(1)), getCellSize(0), getCellSize(1) };
				if (SDL_PointInRect(&point, &thisRect))
				{
					ghost->die();
					addScore(200);

					//Si era un smartGhost muerto, lo eliminamos de la lista y lo borramos
					SmartGhost* smartGhost = dynamic_cast<SmartGhost*>(ghost);
					if (smartGhost != nullptr && smartGhost->isDead())
					{
						stage.erase(it);
						delete smartGhost;
					}
				}
			}
			it++;
		}
	}

	else
		pac->handleEvent(e);
}

bool PlayState::nextCell(int x, int y)
{
	//No hay muro
	if (map->getCell(y, x) != Wall)
		return true;
	else
		return false;
}

bool PlayState::eatFood(int x, int y)
{
	bool vitamin = false;
	if (map->getCell(y, x) != Empty)
	{
		if (map->getCell(y, x) == Vitamins)
			vitamin = true;
		//Se come la comida
		map->eatFood(y, x);
		addScore(10);
	}
	return vitamin;
}

void PlayState::killPacman()
{
	pac->die();
	//Para que quede bonito
	if (!gameover)
	{
		list<GameObject*>::iterator it = stage.begin();
		it++; it++;
		while (it != stage.end())
		{
			GameCharacter* personaje = dynamic_cast<GameCharacter*>(*it);
			if (personaje != nullptr)
			{
				personaje->die();
				it++;
			}
			
		}
		SDL_RenderClear(juego->getRenderer());
		render();
		hud->renderReady();
		//SDL_Delay(READY_DELAY);
	}
}

bool PlayState::hayFantasma(int x, int y, list<GameObject*>::iterator & it)
{
	it = stage.begin();
	it++; it++; //Para no contar a PacMan ni al mapa
	bool fantasma = false;
	while (it != stage.end() && !fantasma)
	{
		Ghost* ghost = dynamic_cast<Ghost*>(*it);
		fantasma = ghost->getPosX() == x && ghost->getPosY() == y;
		if (!fantasma)
			it++;
	}
	return fantasma;
}

void PlayState::createBabyGhost(unsigned int x, unsigned int y)
{
	SmartGhost* baby = new SmartGhost(juego->getTexture(CharactersText), juego, this, x, y);
	stage.push_back(baby);
}

void PlayState::deleteGhosts()
{
	list<GameObject*>::iterator it = stage.begin();
	it++; it++; //Para no contar a PacMan ni al mapa

	while (it != stage.end())
	{
		delete (*it);
		it = stage.erase(it);
	}
}

void const PlayState::compruebaLimites(int &x, int &y)
{
	if (x >= int(map->getSize(1)))
		x = 0;
	else if (x < 0)
		x = map->getSize(1) - 1;
	else if (y >= int(map->getSize(0)))
		y = 0;
	else if (y < 0)
		y = map->getSize(0) - 1;
}

void PlayState::killGhost(list<GameObject*>::iterator it)
{
	GameCharacter* personaje = dynamic_cast<GameCharacter*>(*it);
	if (personaje != nullptr)
	{
		personaje->die();
		addScore(200);

		//Si era un smartGhost muerto, lo eliminamos de la lista y lo borramos
		SmartGhost* smartGhost = dynamic_cast<SmartGhost*>(personaje);
		if (smartGhost != nullptr && smartGhost->isDead())
		{
			stage.erase(it);
			delete smartGhost;
		}
	}
}

void PlayState::loadMap(string filename)
{
	try
	{
		bool archivoGuardado = (filename[0] != 'l'); //Para las partidas guardadas

		filename = LEVELS_PATH + filename;
		ifstream archivo;

		archivo.open(filename);


		if (archivo.is_open())
		{
			//Si era una partida guardada, hay que leer el nivel y puntuación
			if (archivoGuardado)
				archivo >> level >> score;

			if (map != nullptr)
			{
				delete map;
				list<GameObject*>::iterator it = stage.begin();
				it++;
				stage.erase(it);
			}

			map = new GameMap(juego->getTexture(WallText), juego->getTexture(FoodText), juego->getTexture(VitaminText), juego, this);
			stage.push_back(map);
			map->loadFromFile(archivo);

			//Inicializamos el tamaño de celda
			cellHeight = juego->getWindowSize(1) / map->getSize(0);
			cellWidth = (juego->getWindowSize(0) / map->getSize(1)) * 4 / 5;

			unsigned int num_ghosts;
			archivo >> num_ghosts;
			unsigned int i = 0; unsigned int ghostNo = 0;
			//Creamos los fantasmas
			while (i < num_ghosts && !archivo.fail())
			{
				Ghost* ghost;
				int ghostType;
				archivo >> ghostType;
				if (ghostType == 0)
				{
					ghost = new Ghost(juego->getTexture(CharactersText), juego, this, ghostNo);
					ghostNo++;
				}

				else
					ghost = new SmartGhost(juego->getTexture(CharactersText), juego, this);
				ghost->loadFromFile(archivo);
				stage.push_back(ghost);
				i++;
			}
			//En todos los niveles menos el 1º
			if (pac == nullptr)
			{
				pac = new Pacman(juego->getTexture(CharactersText), juego, this);
				stage.insert(stage.begin(), pac);
			}

			pac->loadFromFile(archivo);
		}
		else
			throw FileNotFoundError("Cannot find file: " + filename);

		archivo.close();
	}

	catch (FileNotFoundError& e)
	{
		juego->getStateMachine()->popState();
		cout << e.what() << endl;
		
	}
	catch (FileFormatError& e)
	{
		juego->getStateMachine()->popState();
		cout << e.what() << endl;	
	}
}


void PlayState::SaveGame(int code)
{
	ofstream file;
	file.open(LEVELS_PATH + to_string(code) + ".pac");
	if (file.is_open())
	{
		//Nivel y puntuación
		file << level << " " << score << endl;
		//Mapa
		map->saveToFile(file);
		//Personajes
		file << stage.size() - 2 << endl;
		list<GameObject*>::reverse_iterator rit = stage.rbegin();
		while (rit != stage.rend() && !file.fail())
		{
			GameCharacter* character = dynamic_cast<GameCharacter*>(*rit);
			if (character != nullptr)
			{
				if (typeid (*character) == typeid (Ghost))
					file << "0 ";
				else if (typeid (*character) == typeid (SmartGhost))
					file << "1 ";
				character->saveToFile(file);
				file << endl;
			}
			rit++;
		}
	}
	file.close();
}
