#include "Actor.h"
//#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Actor::Actor(Game* game)
:mGame(game)
, mState(Actor::EAlive)
, mPosition(Vector2::Zero)
, mScale(Vector2(32, 32))
, mSprite(nullptr)
, mColl(nullptr)
,mSpeed(0.0f)
, mDimension(32)
{
	mGame->AddActor(this);
	mColl = new CollisionComponent(this);
}

Actor::~Actor() {
	if (mSprite != nullptr) {
		mSprite->destroy();
	}
	if (mColl != nullptr) {
		delete mColl;
	}
	mGame->RemoveActor(this);
}

void Actor::Draw()
{
	mSprite->draw(static_cast<int>(mPosition.x), static_cast<int>(mPosition.y));
}
