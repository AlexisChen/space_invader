#include "game.h"
#include "algorithm"
#include <fstream>
#include <string>
#include "Actor.h"
#include "SpriteComponent.h"

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
		float move = deltaTime * 160.0f;

		//process input
		IDiceInvaders::KeyStatus keys;
		mSystem->getKeyStatus(keys);
		if (keys.right)
			horizontalPosition += move;
		else if (keys.left)
			horizontalPosition -= move;
		//update game
		UpdateGame();
		
		GenerateOutput();
		
	}
}
void Game::ShutDown() 
{
	mSystem->destroy();
}

void Game::ProcessInput()
{
}
void Game::UpdateGame() 
{
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
	int posX = 32;
	int posY = 16;
	while (!std::getline(infile, line).eof())
	{
		for (char& c : line) {
			std::shared_ptr<Actor> actor = nullptr;
			switch (c) {
				case 'A':
				{
					actor = std::make_unique<Actor>(this);
					actor->SetSprite(mSystem->createSprite("data/enemy1.bmp"));
					
					break;
				}
				case 'B':
				{
					actor = std::make_unique<Actor>(this);
					actor->SetSprite(mSystem->createSprite("data/enemy2.bmp"));
					break;
				}
				case 'C':
				{
					actor = std::make_unique<Actor>(this);
					actor->SetSprite(mSystem->createSprite("data/player.bmp"));
					break;
				}
				default:
					break;
			}
			if (actor != nullptr) {
				actor->SetPosition(Vector2(posX, posY));
			}
			posX += 32;
		}
		posX = 32;
		posY += 16;
	
	}
}
void Game::UnLoadData() 
{}

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
void Game::AddActor(std::shared_ptr<Actor> actor)
{
	mActors.push_back(actor);
}
void Game::RemoveActor() 
{
}