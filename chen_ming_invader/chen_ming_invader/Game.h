#pragma once
#include "DiceInvaders.h"
#include <cassert>
#include <cstdio>
#include <windows.h>
#include <vector>
#include <memory>

class SpriteComponent;
class Actor;

using SPRITEVEC = std::vector<std::shared_ptr<SpriteComponent> >;
using ACTORVEC = std::vector<std::shared_ptr<Actor> >;

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
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();  //load the level file
	void UnLoadData(); //unload the actors.
	void AddSprite(SpriteComponent* sc);
	void RemoveSprite(SpriteComponent* sc);
	void AddActor(std::shared_ptr<Actor> actor);
	void RemoveActor();

private:
	DiceInvadersLib mLib;
	IDiceInvaders* mSystem;//diceinvader system
	std::vector<SpriteComponent*> mSprites;//a vector of all sprites
	//SPRITEVEC mSprites;
	ACTORVEC mActors;
	float mLastTime;//to keep track of time of last frame;

};
