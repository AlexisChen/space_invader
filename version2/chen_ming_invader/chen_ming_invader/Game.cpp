#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "InvaderFrame.h"
#include <fstream>
#include <string>

Game::Game()
:mLib("DiceInvaders.dll")
,mSystem(nullptr)
,mPlayer(nullptr)
,mFrame(nullptr)
,enemy1OnStartPage(nullptr)
,enemy2OnStartPage(nullptr)
,mLastTime(0.0f)
,mWindowWidth(640)
, mWindowHeight(480)
,mCurrentScore(0)
,mHighestScore(0)
, mCurrentHealth(3)
, mGameOver(false)
,mGameWin(false)
, mGameStarted(false)
{}

bool Game::Initialize() 
{
	//initialize the game window
	mSystem = mLib.get();
	mSystem->init(mWindowWidth, mWindowHeight);
	enemy1OnStartPage = mSystem->createSprite("data/enemy1.bmp");
	enemy2OnStartPage = mSystem->createSprite("data/enemy2.bmp");
	LoadData();
	return true;

}
void Game::RunLoop() 
{
	while (mSystem->update())
	{
		//keep a frame rate at about 60fps, get delta time
		while (mSystem->getElapsedTime() - mLastTime < 0.016) {}
		float currentTime = mSystem->getElapsedTime();
		float deltaTime = currentTime - mLastTime;
		mLastTime = currentTime;
		//get keyboard input
		IDiceInvaders::KeyStatus keys;
		mSystem->getKeyStatus(keys);
		
		if (!mGameOver && !mGameWin && mGameStarted)
		{//process input and update in mid of game
			ProcessInput(keys);
			UpdateGame(deltaTime);
		}		
		GenerateOutput();

		if (!mGameStarted && keys.fire)
		{
			mGameStarted = true;
			enemy1OnStartPage->destroy();
			enemy2OnStartPage->destroy();
		}
		if ((mGameOver||mGameWin) && keys.fire) {
			//restart game from beginning
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
	int actSize = mActors.size();
	for (int i = 0; i < actSize; ++i)
	{
		mActors[i]->ProcessInput(keys);
	}
}

void Game::UpdateGame(float deltaTime) 
{
	int actSize = mActors.size();
	for (int i = 0; i < actSize; ++i)
	{
		auto act = mActors[i];
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
		for (auto act : mActors)
		{
			act->Draw();
		}
		DrawWord();
	}
}

void Game::LoadData() 
{
	//initialize /reinitialize game state variable
	mCurrentScore = 0;
	mCurrentHealth = 3;
	mGameOver = false;
	mGameWin = false;
	//load player
	Vector2 playerPosition(1.0f*mWindowWidth/2 - 16, 410.0f);
	Player* p = new Player(this);
	p->SetPos(playerPosition);
	mPlayer = p;
	//load enemies to invader frame from level file
	Frame* f = new Frame(this);
	f->SetMaxLevel(2);
	f->LoadEnemies();
	mFrame = f;
}

void Game::UnLoadData() 
{
	while (!mActors.empty())
	{
		delete mActors.back();
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

void Game::DrawStartPage()
{
	int posX = 100;
	int posY = 100;
	mSystem->drawText(posX, posY, "Welcome to Space Invader");
	posY += 32;
	enemy1OnStartPage->draw(posX, posY);
	posX += 64;
	mSystem->drawText(posX, posY, " = 30 Schmeckles");	
	posY += 32;
	mSystem->drawText(posX, posY, " = 60 Schmeckles");
	posX -= 64;	
	enemy2OnStartPage->draw(posX, posY);
	posY += 64;
	mSystem->drawText(posX, posY, "Press Space to Start");
	posY += 64;
	mSystem->drawText(posX, posY, "Good Luck");
}