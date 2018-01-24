#pragma once
#include "Actor.h"
class Rocket : public Actor
{
public:
	Rocket(Game* game);
	~Rocket() {}
	void Update(float deltaTime) override;
};