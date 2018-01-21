#pragma once
#include "Math.h"
#include <string>
#include <memory>
#include "DiceInvaders.h"

class Game;
class ISprite;
class SpriteComponent;
class MoveComponent;
class CollisionComponent;
class Actor 
{
public:
	Actor(Game* game);
	~Actor();
	enum State {
		EAlive,
		EDead
	};
	void SetSprite(ISprite* isp);
	virtual void ProcessInput(IDiceInvaders::KeyStatus key) {}
	virtual void Update(float deltaTime) {}
	Game* mGame;
	Actor::State mState;

	void SetPos(Vector2 pos) { mPosition = pos; }
	Vector2 GetPos() { return mPosition; }
	void SetScale(Vector2 scale) { mScale = scale; }
	Vector2 GetScale() { return mScale; }
	CollisionComponent* GetCollision() { return mColl; }
	

protected:
	Vector2 mPosition;
	Vector2 mScale;
	SpriteComponent* mSprite;
	//MoveComponent* mMove;
	CollisionComponent* mColl;
	int mDrawOrder;
	float mSpeed;
};