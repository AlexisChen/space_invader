#include "Bomb.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Player.h"

Bomb::Bomb(Game* game)
:Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetSprite(mGame->GetSprite("data/bomb.bmp"), 10);
	mSpeed = 3.0f;
}


void Bomb::Update(float deltaTime)
{
	SetPos(GetPos() + Vector2(0, mSpeed));
	//when rocket goes out of screen, set state to dead.
	float deadLine = 420.0f;
	if (GetPos().y > deadLine) {
		mState = Actor::EDead;
	}
	//detect collision with all enemies, set state to dead when hit one.
	Player* p = mGame->GetPlayer();
	if (mColl->Intersect(p->GetCollision())) {
		mState = Actor::EDead;
		p->Damage();
	}

}