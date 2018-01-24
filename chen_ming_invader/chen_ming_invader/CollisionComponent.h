#pragma once
#include "Component.h"
#include "Math.h"

class CollisionComponent : public Component 
{
public:
	CollisionComponent(Actor* actor);
	Vector2 GetMin() const ;
	Vector2 GetMax() const ;
	bool Intersect(CollisionComponent* other);
};