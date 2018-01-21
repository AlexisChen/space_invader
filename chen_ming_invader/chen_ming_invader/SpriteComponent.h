#pragma once
#include "Component.h"
#include <string>
#include <memory>

class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* actor);
	~SpriteComponent();
	void SetSprite(ISprite* isp, int drawOrder);
	void Draw();
	int drawOrder;
private: 
	ISprite * mSprite;
};