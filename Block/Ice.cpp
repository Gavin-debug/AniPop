#include"Ice.h"
#include"Data/Constant.h"

Ice *Ice::create(int row, int column)
{
	Ice *spr = new(std::nothrow) Ice;
	
	if (spr&&spr->init(row, column))
	{
		spr->autorelease();
		return spr;
	}
	
	CC_SAFE_DELETE(spr);
	
	return nullptr;
}

bool Ice::init(int row, int column)
{
	if (!Sprite::init())
	{
		return false;
	}
	
	_row = row;
	
	_column = column;
	
	initWithSpriteFrameName("Ice_1.png");
	
	return true;
}

void Ice::iceBroken()
{
	
	stopAllActions();
	
	auto animation = Animation::create();
	
	animation = AnimationCache::getInstance()->getAnimation("Ice");
	
	auto animate = Animate::create(animation);

	runAction(Sequence::create(animate,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, this)),
		NULL));
}
