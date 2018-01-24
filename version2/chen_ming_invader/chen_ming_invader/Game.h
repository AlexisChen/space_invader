#pragma once
#include "DiceInvaders.h"
#include <cassert>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Math.h"

class Actor;
class Player;
class Frame;

class DiceInvadersLib
{
public:
	explicit DiceInvadersLib(const char* libraryPath)
	{
		m_lib = LoadLibrary(libraryPath);
		assert(m_lib);

		DiceInvadersFactoryType* factory = (DiceInvadersFactoryType*)GetProcAddress(
			m_lib, "DiceInvadersFactory");
		m_interface = factory();
		assert(m_interface);
	}

	~DiceInvadersLib()
	{
		FreeLibrary(m_lib);
	}

	IDiceInvaders* get() const
	{
		return m_interface;
	}

private:
	DiceInvadersLib(const DiceInvadersLib&);
	DiceInvadersLib& operator=(const DiceInvadersLib&);

private:
	IDiceInvaders * m_interface;
	HMODULE m_lib;
};


class Game {
public:
	Game();
	~Game() {}
	bool Initialize();
	void RunLoop();//execute the game loop while game is alive
	void ShutDown();
	void ProcessInput(IDiceInvaders::KeyStatus keys);
	void UpdateGame(float deltaTime);
	void GenerateOutput();
	void LoadData();  
	void UnLoadData();
	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);
	ISprite* GetSprite(std::string fileName);
	Player* GetPlayer() { return mPlayer; }
	Frame* GetFrame() { return mFrame; }
	void GameOver() { mGameOver = true; }
	void GameWin() { mGameWin = true; }
	void Score(int s);//increment current score, update highest if needed.
	Vector2 GetWinSize() { return Vector2(1.0f*mWindowWidth, 1.0f* mWindowHeight);  }
	
private:
	void DrawWord();
	void DrawStartPage();
	DiceInvadersLib mLib;
	IDiceInvaders* mSystem;//diceinvader system
	std::vector<Actor*> mActors;
	Player* mPlayer;
	Frame* mFrame;
	ISprite* enemy1OnStartPage;
	ISprite* enemy2OnStartPage;
	float mLastTime;//to keep track of time of last frame;
	int mWindowWidth;
	int mWindowHeight;
	int mCurrentScore;
	int mHighestScore;
	int mCurrentHealth;
	bool mGameOver;
	bool mGameWin;
	bool mGameStarted; 
};
