#pragma once
#include "Component.h"
#include "math.h"

class  MoveComponent: public Component
{
public:
	MoveComponent(Actor* actor);
private:
	Vector2 mSpeed;
};