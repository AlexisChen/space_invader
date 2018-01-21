#include "Actor.h"
#include "SpriteComponent.h"

Actor::Actor(Game* game)
:mGame(game)
, mPosition(Vector2::Zero)
, mScale(Vector2::Zero)
, mSprite(nullptr)
, mMove(nullptr)
, mColl(nullptr)
, mDrawOrder(0)
{}

Actor::~Actor() {
}
enum State {
	EAlive,
	EDead
};
void Actor::SetSprite(ISprite* isp)
{
	mSprite = std::make_unique<SpriteComponent>(this);
	mSprite->SetSprite(isp, mDrawOrder);
}
void Actor::ProcessInput()
{
}
void Actor::Update()
{
}

