#include "game.h"
#include "algorithm"
#include <fstream>
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

Game::Game()
:mWindowWidth(640)
,mWindowHeight(480)
,mLib("DiceInvaders.dll")
,mSystem(nullptr)
,mLastTime(0.0f)
,mPlayer(nullptr)
, mGameOver(false)
,mCurrentScore(0)
,mHighestScore(0)
, mCurrentHealth(3)
, mLevel(0)
,mGameWin(false)
, mGameStarted(false)
{}

bool Game::Initialize() 
{
	//initialize the game window
	mSystem = mLib.get();
	mSystem->init(mWindowWidth, mWindowHeight);
	LoadData();
	return true;

}
void Game::RunLoop() 
{
	while (mSystem->update())
	{
		//keep a frame rate at about 60fps
		while (mSystem->getElapsedTime() - mLastTime < 0.016) {}
		float currentTime = mSystem->getElapsedTime();
		float deltaTime = currentTime - mLastTime;
		mLastTime = currentTime;

		IDiceInvaders::KeyStatus keys;
		mSystem->getKeyStatus(keys);
		
		if (!mGameOver && !mGameWin && mGameStarted)
		{
			ProcessInput(keys);
			UpdateGame(deltaTime);
		}		
		
		GenerateOutput();
		if (!mGameStarted && keys.fire)
		{
			mGameStarted = true;
		}
		if ((mGameOver||mGameWin) && keys.fire) {
			//when space if pressed restart game
			UnLoadData();
			LoadData();
		}

	}
}
void Game::ShutDown() 
{
	UnLoadData();
	mSystem->destroy();
}

void Game::ProcessInput(IDiceInvaders::KeyStatus keys)
{
	for (auto act : mActors)
	{
		act->ProcessInput(keys);
	}
}
void Game::UpdateGame(float deltaTime) 
{
	for (auto act : mActors) 
	{
		act->Update(deltaTime);
	}
	std::vector<Actor*> deadActor;
	for (auto act : mActors)
	{
		if (act->mState == Actor::EDead)
		{
			deadActor.push_back(act);
		}
	}
	for (auto dead : deadActor)
	{
		delete dead;
	}
}

void Game::GenerateOutput() 
{
	if (!mGameStarted) 
	{
		DrawStartPage();
	}
	else
	{
		for (auto sp : mSprites)
		{
			sp->Draw();
		}
		DrawWord();
	}
	
	
}

void Game::LoadData() 
{
	//initialize /reinitialize game state variable
	mCurrentScore = 0;
	mCurrentHealth = 3;
	mLevel = 0;
	mGameOver = false;
	mGameWin = false;
	//load player
	Vector2 playerPosition(mWindowWidth/2 - 16, 410);
	Player* p = new Player(this);
	p->SetSprite(mSystem->createSprite("data/player.bmp"));
	p->SetPos(playerPosition);
	mPlayer = p;
	//load enemies from level file
	LoadLevel(mLevel);
}

void Game::UnLoadData() 
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	mEnemies.clear();
}

void Game::AddSprite(SpriteComponent* sc)
{
	mSprites.push_back(sc);

}
void Game::RemoveSprite(SpriteComponent* sc)
{
	auto it = std::find(mSprites.begin(), mSprites.end(), sc);
	if (it != mSprites.end()) {
		mSprites.erase(it);
	}
}

void Game::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}
void Game::RemoveActor(Actor* actor)
{
	auto it = std::find(mActors.begin(), mActors.end(), actor);
	if (it != mActors.end()) {
		mActors.erase(it);
	}
}

ISprite* Game::GetSprite(std::string fileName)
{
	return mSystem->createSprite(fileName.c_str() ); 
}

void Game::RemoveEnemies(Enemy* e)
{
	auto it = std::find(mEnemies.begin(), mEnemies.end(), e);
	if (it != mEnemies.end())
	{
		mEnemies.erase(it);
	}
	if (mEnemies.empty())//all enemies are defeated
	{
		++mLevel;
		if (mLevel > 2)
		{
			mGameWin = true;
		}
		else 
		{
			LoadLevel(mLevel);
		}
	}
}

void Game::GameOver() 
{
	//change game state;
	mGameOver = true;
}

void Game::DrawWord() {
	//(32, 32) is the start position of text
	int posX = 8;
	int posY = 8;
	
	mSystem -> drawText(posX, posY, ("Score: " + std::to_string(mCurrentScore)).c_str() );
	posX += mWindowWidth/2;
	mSystem->drawText(posX, posY, ("Highest Score: " + std::to_string(mHighestScore)).c_str() );
	if (mGameOver) 
	{
		posX -= 48;
		posY += 32;
		mSystem->drawText(posX, posY, "Game Over");
		posX -= 32;
		posY += 32;
		mSystem->drawText(posX, posY, "Press Space to Restart");
	}
	else if (mGameWin)
	{
		posX -= 48;
		posY += 32;
		mSystem->drawText(posX, posY, "You Win");
		posX -= 32;
		posY += 32;
		mSystem->drawText(posX, posY, "Press Space to Restart");
	}
	
	posX = 8;
	posY = mWindowHeight - 32;
	mSystem->drawText(posX, posY, ("Life: " + std::to_string(mCurrentHealth)).c_str() );
	posY -= 16;
	mSystem->drawText(posX, posY, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
		 "- - - - - - - - - - - - - - - - - - - - - - - - "
		"- - - - - - - - - - - - - - - - - - - - - - - - ");
}

void Game::Score(int s)
{
	if (s < 0)//
	{
		-- mCurrentHealth;
	}
	else 
	{
		mCurrentScore += s;
		mHighestScore = max(mCurrentScore, mHighestScore);
	}
}

void Game::LoadLevel(int levelNum)
{
	std::ifstream infile(("data/level"+std::to_string(levelNum)+".txt").c_str());
	std::string line;
	//start position for enemies
	float posX = 16.0f;
	float posY = 64.0f;
	float timeOffset = 1.0f;
	while (!std::getline(infile, line).eof())
	{
		std::cout << line << std::endl;
		for (char& c : line) {
			if (c != '.') 
			{
				Enemy* actor = new Enemy(this);
				if (c == 'A')
				{
					actor->SetSprite(mSystem->createSprite("data/enemy1.bmp"));
					actor->SetScore(30);
				}
				else if (c == 'B')
				{
					actor->SetSprite(mSystem->createSprite("data/enemy2.bmp"));
					actor->SetScore(60);
				}

				mEnemies.push_back(actor);
				actor->SetPos(Vector2(posX, posY));
				actor->SetTimeOffset(timeOffset);
			}
			posX += 32;
		}
		posX = 16;
		posY += 32;
		timeOffset -= 0.1f;
	}
}

void Game::DrawStartPage()
{
	int posX = 100;
	int posY = 100;
	mSystem->drawText(posX, posY, "Welcome to Space Invader");
	posY += 32;
	mSystem->createSprite("data/enemy1.bmp")->draw(posX, posY);
	posX += 64;
	mSystem->drawText(posX, posY, " = 30 Schmeckles");	
	posY += 32;
	mSystem->drawText(posX, posY, " = 60 Schmeckles");
	posX -= 64;
	mSystem->createSprite("data/enemy2.bmp")->draw(posX, posY);
	posY += 64;
	mSystem->drawText(posX, posY, "Press Space to Start");
	posY += 64;
	mSystem->drawText(posX, posY, "Good Luck");


}