#include "Player.h"
#include "Rocket.h"
#include "Math.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Enemy.h"

Player::Player(Game* game) 
:Actor(game)
,mLeading_edge(true)
,mHealth(3)
,mLastBomb(0.0f)
{}

void Player::ProcessInput(IDiceInvaders::KeyStatus key)
{
	if (key.left) 
	{
		mSpeed = -160.0f;
	}else if (key.right) 
	{
		mSpeed = 160.0f;
	}else 
	{
		mSpeed = 0.0f;
	}

	if (key.fire && mLeading_edge && mLastBomb > 0.8f) 
	{
		mLastBomb = 0.0f;
		mLeading_edge = false;
		Rocket* rocket = new Rocket(mGame);
		rocket->SetPos(GetPos() + Vector2(0, -32) );
		rocket->SetScale(Vector2(8, 32) );
	}
	else if (!key.fire) {
		mLeading_edge = true;
		
	}
}

void Player::Update(float deltaTime) 
{
	mLastBomb += deltaTime;
	float posX = GetPos().x + mSpeed * deltaTime;
	posX = max(posX, 64);
	posX = min(posX, mGame->GetWinSize().x-64);
	SetPos(Vector2(posX, GetPos().y) );
	for (auto e : mGame->GetEnemies())
	{
		if (mColl->Intersect(e->GetCollision() ) ) 
		{
			Damage();
		}
	}
}

void Player::Damage()
{
	mHealth--;
	mGame->Score(-1);
	//when damaged three times player die
	if (mHealth == 0) 
	{
		mState = Actor::EDead;
		mGame->GameOver();
	}
}