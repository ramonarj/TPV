#pragma once
#include "GameObject.h"
#include "Vector2D.h"

class BulletsManager
{
public:
	BulletsManager();
	~BulletsManager();
	
	virtual void shoot(GameObject* o, Vector2D p, Vector2D v) = 0;
};

