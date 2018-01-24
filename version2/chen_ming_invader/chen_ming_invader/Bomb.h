#pragma once
#include "Actor.h"
class Game;

class Bomb : public Actor
{
public:
	Bomb(Game* game);
	void Update(float deltaTime) override;
};