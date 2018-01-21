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
	float horizontalPos = mOwner->GetPos().x - mOwner->GetScale().x*1.0f / 2;
	float verticalPos = mOwner->GetPos().y - mOwner->GetScale().y*1.0f / 2;
	mSprite->draw(horizontalPos, verticalPos);
}
