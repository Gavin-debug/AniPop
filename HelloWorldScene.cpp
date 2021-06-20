/*
   * 欢迎游戏界面
   * 功能
        * 通向设置界面
		* 通向游戏选择关卡界面
		* 退出程序
*/

#include "HelloWorldScene.h"
#include "Scene/SelectLevelScene.h"
#include "Scene/SetVolumeScene.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
	//BGM
    if (AudioEngine::getState(worldBgmId) != AudioEngine::AudioState::PLAYING) {
        worldBgmId = AudioEngine::play2d("Resources/Audio/WorldBGM.mp3", true);
    }
	
	auto startItem = MenuItemImage::create(
		"Resources/HelloWorld/StartNormal.png",
		"Resources/HelloWorld/StartSelected.png",
		CC_CALLBACK_1(HelloWorld::menuStartCallBack, this));

	if (startItem == nullptr ||
		startItem->getContentSize().width <= 0 ||
		startItem->getContentSize().height <= 0)
	{
		problemLoading("'StartNormal.png' and 'StartSelected.png'");
	}
	else
	{
		startItem->setPosition(Vec2(150.0, 250.0));
	}

	//退出游戏按钮
	auto closeItem = MenuItemImage::create(
		"Resources/HelloWorld/CloseNormal.png",
		"Resources/HelloWorld/CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallBack, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		closeItem->setPosition(Vec2(150.0, 150.0));
	}

	//设置按钮
	auto setItem = MenuItemImage::create(
		"Resources/HelloWorld/SettingNormal.png",
		"Resources/HelloWorld/SettingSelected.png",
		CC_CALLBACK_1(HelloWorld::menuSetCallback, this));

	float x = origin.x + setItem->getContentSize().width / 2;
	float y = origin.y + visibleSize.height - setItem->getContentSize().height / 2;
	setItem->setPosition(Vec2(x, y));

	auto menu = Menu::create(startItem, closeItem, setItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//游戏背景
	auto sprite = Sprite::create("Resources/HelloWorld/WorldBG.jpg");
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld/WorldBG.jpg'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}

    return true;
}

//通向选择关卡界面
void HelloWorld::menuStartCallBack(cocos2d::Ref * pSender)
{
    AudioEngine::stop(worldBgmId);
    auto nextScene = SelectLevel::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;
}

//退出游戏
void HelloWorld::menuCloseCallBack(Ref* pSender)
{
    AudioEngine::stopAll();
    Director::getInstance()->end();

}

//通向设置界面
void HelloWorld::menuSetCallback(Ref *pSender)
{
	auto nextScene = SetVolume::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;
}


