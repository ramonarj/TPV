#include "PacManObject.h"

//Constructora
PacManObject::PacManObject(): GameObject(nullptr), playState(nullptr){}
PacManObject::PacManObject(Game* juego, PlayState* playState) : GameObject(juego), playState(playState){}


PacManObject::~PacManObject(){}
