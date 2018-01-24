#include "Rocket.h"
#include "Game.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "InvaderFrame.h"

Rocket::Rocket(Game* game) 
:Actor(game)
{
	mSpeed = -3.0f;
	mSprite = game->GetSprite("data/rocket.bmp");
}

void Rocket::Update(float deltaTime)
{
	SetPos(GetPos() + Vector2(0, mSpeed));
	//when rocket goes out of screen, set state to dead.
	if (GetPos().y < 0) {
		mState = Actor::EDead;
	}
	Frame* invaderFrame = mGame->GetFrame();
	if (mColl->Intersect(invaderFrame->GetCollision())) {
		if (invaderFrame->DetectRocket(GetPos()))
		{
			mState = Actor::EDead;
		}
	}
}