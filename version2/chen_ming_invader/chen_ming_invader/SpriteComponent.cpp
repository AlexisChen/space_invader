#include "SpriteComponent.h"
#include "Game.h"
#include "Actor.h"

SpriteComponent::SpriteComponent(Actor* actor)
:Component(actor)
,mSprite(nullptr)
{
	mOwner->mGame->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->mGame->RemoveSprite(this);
	mSprite->destroy();
}
void SpriteComponent::SetSprite(ISprite* isp, int drawOrder)
{
	mSprite = isp;
	this->drawOrder = drawOrder;
}
void SpriteComponent::Draw()
{
	float horizontalPos = mOwner->GetPos().x;
	float verticalPos = mOwner->GetPos().y ;
	mSprite->draw(horizontalPos, verticalPos);
}
