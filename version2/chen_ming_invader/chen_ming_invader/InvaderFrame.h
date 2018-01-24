#pragma once
#include "Actor.h"
#include "Math.h"
#include <vector>
#include <random>

//to optimize the invaders, include them in a frame
//when detecting collision, locate the enemy inside the frame
//instead of looping through the vector of enemies,
//and collide the rocket with them

class Game;
//class Enemy;
struct ISprite;
class Frame :public Actor
{
public:
	Frame(Game* game);
	~Frame();
	void Update(float deltaTime)override;
	void Draw()override; //called from Game::GenerateOutput();
	void LoadEnemies(); //load enemies from given level
	void UnLoadEnemies();
	void SetMaxLevel(int ml) { mMaxLevel = ml; }
	void GenerateBomb();
	bool DetectRocket(Vector2 rocketLocation); //locate rocket and destroy enemy if hit by rocket
	
private:
	
	int GetEnemyIndex(Vector2 rocketLocation); //get the index of enemy hit by rocket
	bool IndentFrameEdge(bool left); //check whether to rescale when left(0)/right(1)-most enemy is hit.
	std::vector<ISprite*> mEnemies;
	std::vector<int> mAliveEnemies;
	std::random_device mRd;
	std::mt19937 mMt;
	float mTimeSinceLastStep;
	float mSecondsPerStep;
	int mLeftMostColumn;
	int mRightMostColumn;
	int mNumRows;
	int mNumCols;
	int mStep;
	int mFramePerBomb;
	int mCurrentLevel;
	int mMaxLevel;
};
