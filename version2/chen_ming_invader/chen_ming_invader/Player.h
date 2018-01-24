#pragma once
#include "Actor.h"
#include "DiceInvaders.h"

class Game;
class Player : public Actor 
{

public:
	Player(Game* game);
	~Player() {}
	void ProcessInput(IDiceInvaders::KeyStatus key) override;
	void Update(float deltaTime) override;
	void Damage();
	int GetHealth() { return mHealth; }
private:
	bool mLeading_edge;
	int mHealth; 
	//time elapsed since last rocket: control player to shoot one rocket per 0.8 sec max
	float mLastBomb;
};