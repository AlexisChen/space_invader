#pragma once
#include "DiceInvaders.h"
#include <memory>

class Actor;
class Component 
{
public: 
	Component(Actor* actor);
	virtual ~Component() {};
	virtual void ProcessInput(IDiceInvaders::KeyStatus key) ;
	virtual void Update(float deltaTime);

protected:
	Actor* mOwner;
};