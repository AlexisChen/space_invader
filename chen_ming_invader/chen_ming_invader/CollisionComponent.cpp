#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(Actor* actor)
	:Component(actor) {}


Vector2 CollisionComponent::GetMin()const
{
	return  Vector2(mOwner->GetPos() - mOwner->GetScale()*0.5f);
}

Vector2 CollisionComponent::GetMax()const
{
	return  Vector2(mOwner->GetPos() + mOwner->GetScale()*0.5f);
}

bool CollisionComponent::Intersect(CollisionComponent* other)
{
	bool intersect;
	Vector2 mMin = GetMin();
	Vector2 mMax = GetMax();
	Vector2 oMin = other->GetMin();
	Vector2 oMax = other->GetMax();
	//on left or right of other
	if (mMax.x < oMin.x || mMin.x > oMax.x)
	{
		intersect = false;
	}
	//on top of or below
	else if (mMax.y < oMin.y || mMin.y > oMax.y ) 
	{
		intersect = false;
	}
	//intersect
	else
	{
		intersect = true;
	}
	return intersect;
}