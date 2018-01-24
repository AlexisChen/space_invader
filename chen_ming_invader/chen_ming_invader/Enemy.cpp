#include "Enemy.h"
#include "Game.h"
#include "Bomb.h"

Enemy::Enemy(Game* game)
	:Actor(game)
	, totalDisplacement(0.0f)
	, maxDisplacement(150.0f)
	,mMt(mRd())
	,mFramePerBomb(2000)
	, mUpdateRate(1.0f)
{
	mSpeed = 8.0f;

}

void Enemy::Update(float deltaTime)
{
	if (!hit) {
		
		mCurrentTime += deltaTime;
		if (mCurrentTime > mUpdateRate) 
		{
			mCurrentTime = 0.0f;
			float displacement = mSpeed;
			SetPos(GetPos() + Vector2(displacement, 0));
			totalDisplacement += abs(displacement);
			//change direction if reach the edge
			if (totalDisplacement >= maxDisplacement) {
				mSpeed *= -1;
				totalDisplacement = 0.0f;
				SetPos(GetPos() + Vector2(0, 8) );
				mUpdateRate -= 0.1f;
			}
		}

		//randomly produce bomb:
		std::uniform_int_distribution<int> dist (0, mFramePerBomb);
		if (dist(mMt) < 3)
		{
			Bomb* b = new Bomb(mGame);
			b->SetPos(GetPos());
			b->SetScale(Vector2(10, 32));
		}
	}
	//hit by a rocket
	else {
		mGame->RemoveEnemies(this);
		mGame->Score(mScore); 
		mState = Actor::EDead;
	}
}

void Enemy::SetTimeOffset(float offset)
{
	mCurrentTime = -offset;
}