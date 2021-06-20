/*
   * 音量设置界面
   * 功能
        * 设置音乐音量
		* 设置音效音量
		* 通向欢迎界面
*/

#include "HelloWorldScene.h"
#include "SetVolumeScene.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* SetVolume::createScene()
{
	return SetVolume::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SetVolume::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//返回按钮 
	auto ReturnItem = MenuItemImage::create(
		"Resources/SetVolume/ReturnNormal.png",
		"Resources/SetVolume/ReturnSelected.png",
		CC_CALLBACK_1(SetVolume::menuReturnCallback, this));

	if (ReturnItem == nullptr ||
		ReturnItem->getContentSize().width <= 0 ||
		ReturnItem->getContentSize().height <= 0)
	{
		problemLoading("'Resources/Return.png'");
	}
	else
	{
		ReturnItem->setPosition(Vec2(100.0,700.0));
	}
	
	//
	auto menu = Menu::create(ReturnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	//游戏背景
	auto sprite = Sprite::create("Resources/SetVolume/SetBG.png");
	if (sprite == nullptr)
	{
		problemLoading("'Resources/SetVolume/SetBG.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}
    
    //设置背景 
	auto setPanel = Sprite::create("Resources/SetVolume/SetPanel.png");
	if (setPanel == nullptr)
	{
		problemLoading("Resources/SetVolume/SetPanel.png");
	}
	else
	{
		// position the sprite on the center of the screen
		setPanel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		// add the sprite as a child to this layer
		this->addChild(setPanel, 1);
	}

	//音乐
	auto music_bgm = Sprite::create("Resources/SetVolume/SetBGM.png");
	music_bgm->setPosition(Vec2(visibleSize.width / 3 + 50, visibleSize.height / 3*2 -50));
	this->addChild(music_bgm, 2);
	
	//音效
	auto effect_bgm = Sprite::create("Resources/SetVolume/SetSoundEffect.png");
	effect_bgm->setPosition(Vec2(visibleSize.width / 3 + 53, visibleSize.height / 3 * 2 - 130));
	this->addChild(effect_bgm, 3);
	
	//音乐滑动条
	auto music_slider = Slider::create();
	music_slider->loadBarTexture("Resources/SetVolume/sliderTrack.png");
	music_slider->loadSlidBallTextures("Resources/SetVolume/sliderballnormal.png", "Resources/SetVolume/sliderballnormal.png", "");
	music_slider->loadProgressBarTexture("Resources/SetVolume/sliderProgress.png");
	float music_percent = UserDefault::getInstance()->getFloatForKey("music_percent");

	/*if (music_percent == 0.0f)
	{
		music_percent = 100.0f;
	}*/

	music_slider->setPercent(music_percent);
	music_slider->setPosition(Vec2(visibleSize.width / 3 + 210, visibleSize.height / 3 * 2 - 50));
	
	music_slider->addEventListener(
		[=](Ref* pSender, Slider::EventType type) {

		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			int percent = music_slider->getPercent();
			AudioEngine::setVolume(worldBgmId, static_cast<float>(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("music_percent", percent);
		}
	}
	);

	this->addChild(music_slider,4);
	
	//音效滑动条
	auto effect_slider = Slider::create();
	effect_slider->loadBarTexture("Resources/SetVolume/sliderTrack.png");
	effect_slider->loadSlidBallTextures("Resources/SetVolume/sliderballnormal.png", "Resources/SetVolume/sliderballnormal.png", "");
	effect_slider->loadProgressBarTexture("Resources/SetVolume/sliderProgress.png");
	float effect_percent = UserDefault::getInstance()->getFloatForKey("effect_percent");

	/*if (effect_percent == 0.0f)
	{
		effect_percent = 100.0f;
	}*/

	effect_slider->setPercent(effect_percent);
	effect_slider->setPosition(Vec2(visibleSize.width / 3 + 210, visibleSize.height / 3 * 2 - 130));

	effect_slider->addEventListener(
		[=](Ref* pSender, Slider::EventType type) {

		if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
			int percent = effect_slider->getPercent();
			AudioEngine::setVolume(bgmId, static_cast<float>(percent) / 100);
			UserDefault::getInstance()->setFloatForKey("effect_percent", percent);
		}
	}
	);

	this->addChild(effect_slider, 5);
	
	return true;
}

//返回
void SetVolume::menuReturnCallback(Ref *pSender)
{
	auto nextScene = HelloWorld::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;
}
