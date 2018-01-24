#pragma once
#include "Actor.h"
#include <random>

class Enemy :public Actor 
{
public:
	Enemy(Game* game);
	void Update(float deltaTime) override;
	bool hit;
	void SetScore(int s) { mScore = s; }
	void SetTimeOffset(float offset);//movement time offset
private: 
	float totalDisplacement;
	float maxDisplacement;
	std::random_device mRd;
	std::mt19937 mMt;
	int mFramePerBomb;
	int mScore; //score added when hit
	float mCurrentTime;//time since last move
	float mUpdateRate;//update once per mUpdateRate;
};