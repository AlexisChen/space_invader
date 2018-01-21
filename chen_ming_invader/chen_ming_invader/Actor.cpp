#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"

Actor::Actor(Game* game)
:mGame(game)
, mState(Actor::EAlive)
, mPosition(Vector2::Zero)
, mScale(Vector2(32, 32))
, mSprite(nullptr)
, mColl(nullptr)
, mDrawOrder(0)
,mSpeed(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor() {
	if (mSprite != nullptr) {
		delete mSprite;
	}
	if (mColl != nullptr) {
		delete mColl;
	}
	mGame->RemoveActor(this);
}
enum State {
	EAlive,
	EDead
};
void Actor::SetSprite(ISprite* isp)
{
	mSprite = new SpriteComponent (this);
	mSprite->SetSprite(isp, mDrawOrder);
}


