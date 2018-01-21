#pragma once
#include "Math.h"
#include <string>
#include <memory>

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
	void ProcessInput();
	void Update();
	Game* mGame;

	void SetPosition(Vector2 pos) { mPosition = pos; }
	Vector2 GetPos() { return mPosition; }
	void SetScale(Vector2 scale) { mScale = scale; }
	Vector2 GetScale() { return mScale; }
	

private:
	Vector2 mPosition;
	Vector2 mScale;
	std::shared_ptr<SpriteComponent> mSprite;
	MoveComponent* mMove;
	CollisionComponent* mColl;
	int mDrawOrder;
};