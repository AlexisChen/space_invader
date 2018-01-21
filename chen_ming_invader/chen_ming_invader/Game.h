#pragma once
#include "DiceInvaders.h"
#include <cassert>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>

class SpriteComponent;
class Actor;
class Enemy;

using SPRITEVEC = std::vector<std::shared_ptr<SpriteComponent> >;
using ACTORVEC = std::vector<std::unique_ptr<Actor> >;

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
	bool Initialize();
	void RunLoop();//execute the game loop while game is alive
	void ShutDown();
	void ProcessInput(IDiceInvaders::KeyStatus keys);
	void UpdateGame(float deltaTime);
	void GenerateOutput();
	void LoadData();  //load the level file
	void UnLoadData(); //unload the actors.
	void AddSprite(SpriteComponent* sc);
	void RemoveSprite(SpriteComponent* sc);
	void AddActor(Actor* actor);
	//void AddActor(std::unique_ptr<Actor> actor);
	void RemoveActor(Actor* actor);
	ISprite* GetSprite(std::string fileName);
	std::vector<Enemy*> GetEnemies() { return mEnemies; }
private:
	DiceInvadersLib mLib;
	IDiceInvaders* mSystem;//diceinvader system
	std::vector<SpriteComponent*> mSprites;//a vector of all sprites
	//SPRITEVEC mSprites;
	std::vector<Actor*> mActors;
	//ACTORVEC mActors;
	float mLastTime;//to keep track of time of last frame;
	std::vector<Enemy*> mEnemies;

};
