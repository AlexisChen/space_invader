#include "InvaderFrame.h"
#include "Game.h"
#include "DiceInvaders.h"
#include "CollisionComponent.h"
#include "Bomb.h"
#include <fstream>

Frame::Frame(Game* game)
:Actor(game)
,mMt(mRd())
,mTimeSinceLastStep(0.0f)
,mSecondsPerStep(1.0f)
,mLeftMostColumn(0)
,mRightMostColumn(-1)
,mNumRows(0)
,mNumCols(0)
,mStep(8)
,mFramePerBomb(20)
,mCurrentLevel(0)
,mMaxLevel(0)
{}

Frame::~Frame()
{
	UnLoadEnemies();
}

void Frame::Update(float deltaTime) 
{
	//move with stepsize
	mTimeSinceLastStep += deltaTime;
	if (mTimeSinceLastStep > mSecondsPerStep) 
	{
		mTimeSinceLastStep = 0.0f;
		SetPos(GetPos() + Vector2(mStep, 0));
		bool hitEdge = false;
		//when hit the edge:
		if (mStep > 0)
		{//moving to the right
			float rightEdge = mColl->GetMax().x;
			//to add: replace mn with window width
			if (rightEdge > (640 - 1.5*mDimension) )
			{
				hitEdge = true;
			}
		}
		else 
		{//moving to the left
			float leftEdge = mColl->GetMin().x;
			//to add: replace mn with window width
			if (leftEdge < 1.5 * mDimension)
			{
				hitEdge = true;
			}
		}
		if (hitEdge)
		{
			mStep *= -1;//switch direction
			SetPos(GetPos() + Vector2(0.0f, 0.5f*mDimension));//step down one line
			mSecondsPerStep *= 0.9f;//speed up
		}
		
	}
	//randomly shoot bomb
	std::uniform_real_distribution<float> dist(0, mFramePerBomb);
	if (dist(mMt) < 1.0f)
	{
		//randomly select position
		std::uniform_int_distribution<int> randomInvader(0, mAliveEnemies.size() - 1);
		int index = mAliveEnemies[randomInvader(mMt)];
		int row = index / mNumCols;
		int col = index % mNumCols;
		Vector2 topLeft = mColl->GetMin() + Vector2(mDimension*1.0f, mDimension*1.0f)*0.5f;
		float xDisplacement = topLeft.x + (-mLeftMostColumn + col) * mDimension;
		float yDisplacement = topLeft.y + row * mDimension;
		Bomb* b = new Bomb(mGame);
		b->SetPos(Vector2(xDisplacement, yDisplacement));
		int bombWidth = 10;
		b->SetScale(Vector2(bombWidth*1.0f, mDimension*1.0f));
	}
	
}

void Frame::LoadEnemies() 
{
	//std::ifstream infile(("data/level" + std::to_string(mCurrentLevel) + ".txt").c_str());
	std::ifstream infile("data/level.txt");
	std::string line;
	Vector2 topLeft(1.5f * mDimension, 2.5f * mDimension);
	int posX = topLeft.x;
	int posY = topLeft.y;
	while (!std::getline(infile, line).eof())
	{
		for (char& c : line) {
			if (c == 'A')
			{
				mEnemies.push_back(mGame -> GetSprite("data/enemy1.bmp"));
			}
			else
			{
				mEnemies.push_back(mGame->GetSprite("data/enemy2.bmp"));
			}
			posX += mDimension;
		}
		posX = static_cast<int>(1.5 * mDimension);
		posY += mDimension;
		++mNumRows;
	}
	mNumCols = mEnemies.size() / mNumRows;
	mRightMostColumn = mNumCols - 1;
	SetScale(Vector2(mNumCols, mNumRows) *mDimension );
	SetPos(topLeft - Vector2(mDimension, mDimension)*0.5 + GetScale() * 0.5 );
	int size = mEnemies.size();
	mAliveEnemies = std::vector<int>(size);
	//populate alive enemies
	std::generate(mAliveEnemies.begin(), mAliveEnemies.end(), [n = 0]() mutable { return n++; });
}

void Frame::UnLoadEnemies() 
{
	for (auto sp : mEnemies)
	{
		if (sp != nullptr)
		{
			sp->destroy();
			sp = nullptr;
		}
		mEnemies.clear();
	}
	mAliveEnemies.clear();
	mTimeSinceLastStep = 0.0f;
	mSecondsPerStep = 1.0f;
	mLeftMostColumn = 0;
	mRightMostColumn = -1;
	mNumRows = 0;
	mNumCols = 0;
}

void Frame::Draw() 
{
	Vector2 topLeft = mColl->GetMin() + Vector2(mDimension, mDimension)*0.5;
	int posX = static_cast<int>(topLeft.x);
	int posY = static_cast<int>(topLeft.y);
	for (int i = 0; i < mNumRows; ++i)
	{
		for (int j = mLeftMostColumn; j <= mRightMostColumn; ++j) 
		{
			int index = i * mNumCols + j;
			if (mEnemies[index] != nullptr)
			{
				mEnemies[index]->draw(posX, posY);
			}
			posX += mDimension;
		}
		posX = static_cast<int>(topLeft.x);
		posY += mDimension;
	}
}
void Frame::GenerateBomb()
{

}

bool Frame::DetectRocket(Vector2 rocketLocation) 
{
	bool rst = false;
	int index = GetEnemyIndex(rocketLocation);
	if (mEnemies[index] != nullptr)
	{//if one of the enemies hit by rocket
		mEnemies[index]->destroy();
		mEnemies[index] = nullptr;
		auto it = std::find(mAliveEnemies.begin(), mAliveEnemies.end(), index);
		if (it != mAliveEnemies.end())
		{
			mAliveEnemies.erase(it);
		}
		if (mAliveEnemies.empty())
		{//all enemies are destroyed
			if (mCurrentLevel = mMaxLevel)
			{//reach max level
				mGame->GameWin();
			}
			else 
			{//increment current level and load new enemies
				++ mCurrentLevel;
				UnLoadEnemies();
				LoadEnemies();
			}
		}
		int col = index % mNumCols;
		if (col == mLeftMostColumn )
		{
 			IndentFrameEdge(true);
		}
		else if (col == mRightMostColumn)
		{
			IndentFrameEdge(false);
		}
		rst = true;
	}
	return rst;
}

int Frame::GetEnemyIndex(Vector2 rocketLocation) 
{
	Vector2 topLeft = mColl->GetMin();
	float xDisplacement = rocketLocation.x - topLeft.x + mLeftMostColumn * mDimension;
	float yDisplacement = rocketLocation.y - topLeft.y;
	int col = static_cast<int>(xDisplacement/mDimension);
	int row = static_cast<int>(yDisplacement / mDimension);
	col = min(max(col, 0), mNumCols - 1);
	row = min(max(row, 0), mNumRows - 1);
	int index = row * mNumCols + col;
	return index;
}

bool Frame::IndentFrameEdge(bool left)
{
	bool indent = true;
	int col = left ? mLeftMostColumn : mRightMostColumn;
	int index = col;
	for (int i = 0; i < mNumRows-1; ++i)
	{
		index += mNumCols;
		if (mEnemies[index] != nullptr)
		{
			indent = false;
			break;
		}
	}
	if (indent) 
	{//recenter and rescale frame when indent happen
		if (left)
		{//indent on the left;
			++ mLeftMostColumn;
			SetPos(GetPos() +Vector2(0.5f * mDimension, 0));
		}
		else 
		{//indent on the right;
			left = false;
			--mRightMostColumn;
			SetPos(GetPos() - Vector2(0.5f * mDimension, 0));
		}
		SetScale(GetScale() - Vector2(mDimension, 0));
		//IndentFrameEdge(left);
	}
	return indent;
}

	