/*
   * 关卡选择界面
   * 功能
        * 通向不同的关卡
		* 回到欢迎界面
*/

#include "SelectLevelScene.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* SelectLevel::createScene()
{
	return SelectLevel::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool SelectLevel::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
//    AudioEngine::stop(worldBgmId);
	//BGM
//    AudioEngine::play2d("Audio/WorldBGM.mp3", true);

	//返回按钮
	MenuItemImage* returnItem = MenuItemImage::create(
		"Resources/SelectLevel/return.png",
		"Resources/SelectLevel/return.png",
		CC_CALLBACK_1(SelectLevel::menuReturnCallBack, this)
	);
	
	if (returnItem == nullptr ||
		returnItem->getContentSize().width <= 0 ||
		returnItem->getContentSize().height <= 0)
	{
		problemLoading("'return.png'");
	}
	else
	{
		float x = origin.x + returnItem->getContentSize().width;
		float y = visibleSize.height - returnItem->getContentSize().height;
		returnItem->setPosition(Vec2(x, y));
	}
	
	MenuItemImage* levelItem_1 = MenuItemImage::create(
		"Resources/SelectLevel/Level_1.png",
		"Resources/SelectLevel/Level_1.png",
		CC_CALLBACK_1(SelectLevel::menuLevelCallBack_1, this)
	);
	
	//第一关按钮
	if (levelItem_1 == nullptr ||
		levelItem_1->getContentSize().width <= 0 ||
		levelItem_1->getContentSize().height <= 0)
	{
		problemLoading("'Level_1.png'");
	}
	else
	{
		float x = origin.x + 80.0;
		float y = visibleSize.height - 550.0;
		levelItem_1->setPosition(Vec2(x, y));
	}

	//第二关按钮
	MenuItemImage* levelItem_2 = MenuItemImage::create(
		"Resources/SelectLevel/Level_2.png",
		"Resources/SelectLevel/Level_2.png",
		CC_CALLBACK_1(SelectLevel::menuLevelCallBack_2, this)
	);
	
	if (levelItem_2 == nullptr ||
		levelItem_2->getContentSize().width <= 0 ||
		levelItem_2->getContentSize().height <= 0)
	{
		problemLoading("'Level_2.png'");
	}
	else
	{
		float x = origin.x + 200.0;
		float y = visibleSize.height - 550.0;
		levelItem_2->setPosition(Vec2(x, y));
	}

	//锁住
	MenuItemImage* lockedItem = MenuItemImage::create(
		"Resources/SelectLevel/Locked.png",
		"Resources/SelectLevel/Locked.png",
		CC_CALLBACK_1(SelectLevel::menuLevelCallBack_3, this)
	);

	if (lockedItem == nullptr ||
		lockedItem->getContentSize().width <= 0 ||
		lockedItem->getContentSize().height <= 0)
	{
		problemLoading("'Locked.png'");
	}
	else
	{
		float x = origin.x + 320.0;
		float y = visibleSize.height - 550.0;
		lockedItem->setPosition(Vec2(x, y));
	}

	Menu* mu = Menu::create(returnItem, levelItem_1, levelItem_2, lockedItem, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu, 1);

	//游戏背景
	auto background = Sprite::create("Resources/SelectLevel/SelectBG.png");
	if (background == nullptr)
	{
		problemLoading("'SelectBG.png.png'");
	}
	else
	{
		background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(background, 0);
	}

	return true;
}

//返回
void SelectLevel::menuReturnCallBack(Ref * pSender)
{
	unscheduleUpdate();
	auto nextScene = HelloWorld::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
}

//第一关
void SelectLevel::menuLevelCallBack_1(cocos2d::Ref * pSender)
{
//	AudioEngine::stop(worldBgmId); //停止播放背景音乐。

	auto nextScene = GameScene::createScene(1);
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;

}

//第二关
void SelectLevel::menuLevelCallBack_2(cocos2d::Ref * pSender)
{
//	AudioEngine::stop(worldBgmId); //停止播放背景音乐。

	auto nextScene = GameScene::createScene(2);
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;
}

void SelectLevel::menuLevelCallBack_3(cocos2d::Ref * pSender)
{
}
