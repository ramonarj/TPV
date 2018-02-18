#include "GameObject.h"
#include "Game.h"

//Constructoras
GameObject::GameObject(){}
GameObject::GameObject(Game*juego) :juego(juego){};


//Destructora
GameObject::~GameObject(){}