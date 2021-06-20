
#include"Animals.h"
#include"Scene/GameScene.h"

Animals::Animals()
	:_imgIndex(0),
	_isNeedRemove(false),
	_isCommonRemove(true),
	_ignoreCheck(false),
	_aniState(COMMON)
{
}

Animals *Animals::create(int row, int column)
{
	Animals *spr = new(std::nothrow) Animals;
	if (spr&&spr->init(row, column))
	{
		spr->autorelease();
		return spr;
	}
	CC_SAFE_DELETE(spr);
	return nullptr;
}

bool Animals::init(int row, int column)
{
	if (!Sprite::init())
	{
		return false;
	}
	_row = row;
	_column = column;
	_imgIndex = rand() % TOTAL_ANI;
	initWithSpriteFrameName(Common[_imgIndex]);
	return true;
}

void Animals::setSelected()
{
	if (_aniState == COMMON)
	{
		stopAllActions();
		auto animation = Animation::create();
		animation = AnimationCache::getInstance()->getAnimation(CommonAni[_imgIndex]);
		auto animate = Animate::create(animation);
		runAction(RepeatForever::create(animate));
	}
}

void Animals::removeSelected()
{
	if (_aniState == COMMON)
	{
		stopAllActions();
		SpriteFrame *frame;
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(Common[_imgIndex]);
//        ParticleSystem* pcs = ParticleSystem::create()
        this->setSpriteFrame(frame);
//        this->setDisplayFrame(frame);
	}
}

void Animals::setState(AnimalsState aniState)
{
	_aniState = aniState;
	
	switch (aniState)
	{
	case VERTICAL:
		this->setVertical();
		break;
	case HORIZONTAL:
		setHorizontal();
		break;
	case BOMB:
		setBomb();
		break;
	case SUPERBIRD:
		setSuperBird();
		break;
	default:
		break;
	}
	return;
}

void Animals::setHorizontal()
{
	stopAllActions();
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation(HorizontalAni[_imgIndex]);
	auto animate = Animate::create(animation);
	runAction(RepeatForever::create(animate));
}

void Animals::setVertical()
{
	stopAllActions();
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation(VerticalAni[_imgIndex]);
	auto animate = Animate::create(animation);
	runAction(RepeatForever::create(animate));
}

void Animals::setBomb()
{
	stopAllActions();
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation(BombAni[_imgIndex]);
	auto animate = Animate::create(animation);
	runAction(RepeatForever::create(animate));
}

void Animals::setSuperBird()
{
	_imgIndex = -1;
	SpriteFrame *frame;
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("SuperBird_1.png");
//	setDisplayFrame(frame);
    this->setSpriteFrame(frame);

}
