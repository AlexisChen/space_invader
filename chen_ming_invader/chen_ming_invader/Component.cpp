#include "Component.h"

Component::Component(Actor* actor)
{
	mOwner = actor; 
}
void Component::ProcessInput(IDiceInvaders::KeyStatus key){}
void Component::Update(float deltaTime){}