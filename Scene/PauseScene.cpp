/*
   * ��Ϸ��ͣ����
   * ����
        * ��������
		* ������Ϸ
		* ���¿�ʼ
		* �˳���Ϸ
*/

#include "PauseScene.h"
#include "GameScene.h"
#include "SelectLevelScene.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

Scene* Pause::createScene()
{
	Scene* scene = Scene::create();
	Pause* pause = Pause::create();
	scene->addChild(pause);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filLayerin HelloWorldScene.cpp\n");
}

bool Pause::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//��Ϸ����
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

	//���ñ��� 
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

	//�˳���Ϸ��ť
	auto BackItem = MenuItemImage::create(
		"Resources/GameOver/Back.jpg",
		"Resources/GameOver/Back.jpg",
		CC_CALLBACK_1(Pause::menuBackCallback, this));
	BackItem->setPosition(Vec2(GAME_SCREEN_WIDTH /2-100, GAME_SCREEN_HEIGHT / 2 -110));

	//������Ϸ��ť
	auto ContinueItem = MenuItemImage::create(
		"Resources/GameOver/Continue.jpg",
		"Resources/GameOver/Continue.jpg",
		CC_CALLBACK_1(Pause::menuContinueCallback, this));
	ContinueItem->setPosition(Vec2(GAME_SCREEN_WIDTH /2, GAME_SCREEN_HEIGHT / 2-110 ));

	//���¿�ʼ��ť
	auto RestartItem = MenuItemImage::create(
		"Resources/GameOver/Restart.jpg",
		"Resources/GameOver/Restart.jpg",
		CC_CALLBACK_1(Pause::menuRestartCallback, this));
	RestartItem->setPosition(Vec2(GAME_SCREEN_WIDTH /2+100, GAME_SCREEN_HEIGHT / 2 -110));

	auto menu = Menu::create(BackItem,ContinueItem,RestartItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	//����ͼ��
	auto music_bgm = Sprite::create("Resources/SetVolume/SetBGM.png");
	music_bgm->setPosition(Vec2(visibleSize.width / 3 + 50, visibleSize.height / 3 * 2 - 50));
	this->addChild(music_bgm, 2);

	//��Чͼ��
	auto effect_bgm = Sprite::create("Resources/SetVolume/SetSoundEffect.png");
	effect_bgm->setPosition(Vec2(visibleSize.width / 3 + 53, visibleSize.height / 3 * 2 - 130));
	this->addChild(effect_bgm, 3);

	//���ֻ�����
	auto music_slider = Slider::create();
	music_slider->loadBarTexture("Resources/SetVolume/sliderTrack.png");
	music_slider->loadSlidBallTextures("Resources/SetVolume/sliderballnormal.png", "SetVolume/sliderballnormal.png", "");
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

	this->addChild(music_slider, 4);

	//��Ч������
	auto effect_slider = Slider::create();
	effect_slider->loadBarTexture("Resources/SetVolume/sliderTrack.png");
	effect_slider->loadSlidBallTextures("Resources/SetVolume/sliderballnormal.png", "SetVolume/sliderballnormal.png", "");
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

//�˳�
void Pause::menuBackCallback(Ref* pSender)
{
	AudioEngine::stopAll(); //ֹͣ���ű������֡�
	auto nextScene = SelectLevel::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = (MenuItem*)pSender;
}

//����
void Pause::menuContinueCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
}

//����
void Pause::menuRestartCallback(Ref* pSender)
{
	auto nextScene = GameScene::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = (MenuItem*)pSender;
}
