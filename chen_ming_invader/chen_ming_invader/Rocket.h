#pragma once
#include "Actor.h"
class Rocket : public Actor
{
public:
	Rocket(Game* game);
	void Update(float deltaTime) override;
};