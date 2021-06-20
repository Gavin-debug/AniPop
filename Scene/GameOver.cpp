/*
   * 游戏结束界面
   * 功能
         * 获得分数
		 * 判断输赢
		 * 重新游戏
		 * 退出游戏
*/

#include "GameOver.h"
#include "GameScene.h"
#include "SelectLevelScene.h"
#include "AudioEngine.h"

Scene* GameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameOver::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//BGM
	AudioEngine::play2d("Resources/Audio/WorldBGM.mp3", true);

	//PlayGround
	auto PlayBG = Sprite::create("Resources/GameOver/PlayBG.png");
	if (PlayBG == nullptr)
	{
		problemLoading("'Resources/GameOver/PlayBG.jpg'");
	}
	else
	{
		PlayBG->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
		this->addChild(PlayBG, 0);
	}
	
	//显示分数的板块
	auto ScoreBG = Sprite::create("Resources/GameOver/ScoBG.png");
	ScoreBG->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2+50));
	this->addChild(ScoreBG, 1);
	
	//重玩的按钮
	auto playAgainItem = MenuItemImage::create(
		"Resources/GameOver/ReplayNormal.png",
		"Resources/GameOver/ReplaySelected.png",
		CC_CALLBACK_1(GameOver::menuReplayCallback, this));
	
	playAgainItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2-100, GAME_SCREEN_HEIGHT / 2-100+25));
	
    //退出游戏的按钮
	auto returnItem = MenuItemImage::create(
		"Resources/GameOver/ReturnNormal.png",
		"Resources/GameOver/ReturnSelected.png",
		CC_CALLBACK_1(GameOver::menuReturnCallback, this));

	returnItem->setPosition(Vec2(GAME_SCREEN_WIDTH / 2 + 100, GAME_SCREEN_HEIGHT / 2 - 100+25));
	
	auto menu = Menu::create(playAgainItem, returnItem,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
	
	//显示分数
	TTFConfig config("Resources/fonts/yuehei.otf", 50);
	auto labelScore = Label::createWithTTF(config, " 0 ");
	labelScore->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	labelScore->setTag(10);
	labelScore->setColor(Color3B::BLACK);
	this->addChild(labelScore, 2);

	//显示输赢
	auto labelAim = Label::createWithTTF(config, " 0 ");
	labelAim->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2 + 100));
	labelAim->setTag(11);
	labelAim->setColor(Color3B::RED);
	this->addChild(labelAim, 2);

	return true;
}

void GameOver::menuReplayCallback(Ref* pSender)
{
	AudioEngine::stopAll(); //停止播放背景音乐。

	auto nextScene = GameScene::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = (MenuItem*)pSender;
}

void GameOver::menuReturnCallback(Ref* pSender)
{
	
	auto nextScene = SelectLevel::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem* item = (MenuItem*)pSender;
}

//获得分数
void GameOver::getScore(int score)
{
	auto labelScore = (Label*)this->getChildByTag(10);
	labelScore->setString(StringUtils::format(" %d ", score));
}

//获得输赢结果
void GameOver::getAim(int aim)
{
	if (aim == 0)
	{
		//无冰块或者指定的动物剩下
		auto labelAim = (Label*)this->getChildByTag(11);
		labelAim->setString(StringUtils::format(" WIN! "));
	}
	else
	{
		auto labelAim = (Label*)this->getChildByTag(11);
		labelAim->setString(StringUtils::format(" LOSE! "));
	}
}
