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
:mLib("DiceInvaders.dll")
,mSystem(NULL)
,mLastTime(0.0f)
{}

bool Game::Initialize() 
{
	//initialize the game window
	mSystem = mLib.get();
	mSystem->init(640, 480);
	LoadData();
	return true;

}
void Game::RunLoop() 
{
	float horizontalPosition = 320;
	
	while (mSystem->update())
	{
		//keep a frame rate at about 60fps
		while (mSystem->getElapsedTime() - mLastTime < 0.016) {}
		float currentTime = mSystem->getElapsedTime();
		float deltaTime = currentTime - mLastTime;
		mLastTime = currentTime;
		//float move = deltaTime * 160.0f;

		//process input
		IDiceInvaders::KeyStatus keys;
		mSystem->getKeyStatus(keys);
		ProcessInput(keys);
		//update game
		UpdateGame(deltaTime);
		
		GenerateOutput();
		
	}
}
void Game::ShutDown() 
{
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
}

void Game::GenerateOutput() 
{
	for ( auto sp : mSprites)
	{
		sp->Draw();
	}
}

void Game::LoadData() 
{
	//mSprites.push_back(mSystem->createSprite("data/player.bmp"));
	std::ifstream infile("data/level.txt");
	std::string line;
	int posX = 16;
	int posY = 16;
	while (!std::getline(infile, line).eof())
	{
		std::cout << line << std::endl;
		for (char& c : line) {
			Actor* actor = nullptr;
			switch (c) {
				case 'A':
				{
					actor = new Enemy( this );
					actor->SetSprite(mSystem->createSprite("data/enemy1.bmp"));
					break;
				}
				case 'B':
				{
					actor = new Enemy( this );
					actor->SetSprite(mSystem->createSprite("data/enemy2.bmp"));
					break;
				}
				case 'C':
				{
					actor = new Player( this );
					actor->SetSprite(mSystem->createSprite("data/player.bmp"));
					break;
				}
				default:
					break;
			}
			if (actor != nullptr) {

				actor->SetPos(Vector2(posX, posY));
			}
			posX += 32;
		}
		posX = 16;
		posY += 32;
	
	}
}
void Game::UnLoadData() 
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
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