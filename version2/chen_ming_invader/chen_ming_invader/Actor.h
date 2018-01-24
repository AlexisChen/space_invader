#pragma once
#include "Math.h"
#include <string>
#include <memory>
#include "DiceInvaders.h"

class Game;
struct ISprite;
//class SpriteComponent;
class MoveComponent;
class CollisionComponent;
class Actor 
{
public:
	
	enum State {
		EAlive,
		EDead
	};
	static int actorDimension;

	Actor(Game* game);
	virtual ~Actor();
	virtual void ProcessInput(IDiceInvaders::KeyStatus key) {}
	virtual void Update(float deltaTime) {}
	virtual void Draw();//draw sprite to screen
	void SetPos(Vector2 pos) { mPosition = pos; }
	void SetScale(Vector2 scale) { mScale = scale; }
	Vector2 GetPos() { return mPosition; }
	Vector2 GetScale() { return mScale; }
	CollisionComponent* GetCollision() { return mColl; }

	Game* mGame;
	Actor::State mState;
	
protected:
	Vector2 mPosition;
	Vector2 mScale;
	ISprite* mSprite;
	CollisionComponent* mColl;
	float mSpeed;
	int mDimension;
};