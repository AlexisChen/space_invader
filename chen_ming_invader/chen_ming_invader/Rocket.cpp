#include "Rocket.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Enemy.h"

Rocket::Rocket(Game* game) 
:Actor(game)
{
	mSpeed = -3.0f;
	mSprite = new SpriteComponent(this);
	mSprite->SetSprite(game->GetSprite("data/rocket.bmp"), 100);
	mColl = new CollisionComponent(this);
}

void Rocket::Update(float deltaTime)
{
	SetPos(GetPos() + Vector2(0, mSpeed));
	//when rocket goes out of screen, set state to dead.
	if (GetPos().y < 0) {
		mState = Actor::EDead;
	}
	//detect collision with all enemies, set state to dead when hit one.
	for (Enemy* e : mGame->GetEnemies()) {
		if (mColl->Intersect(e->GetCollision() ) ) {
			mState = Actor::EDead;
			e->hit = true;
			break;
		}
	}
	
}