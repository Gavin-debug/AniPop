/*
   * 游戏主界面
   * 功能
        * 创建、使用道具
		* 记录分数
		* 显示剩余的步数
		* 显示剩余的目标量
		* 触摸、消除、创建小动物
		* 相关特效，如爆炸等
		* 弹窗功能
		* 暂停游戏功能
		* 退出游戏回到选择关卡界面
		* 传递分数和游戏结果至gameover界面
*/

#include "GameScene.h"
#include "SelectLevelScene.h"
#include "GameOver.h"
#include "PauseScene.h"
#include "Block/Animals.h"
#include "Block/Ice.h"
#include "Dialog/dialog.h"
#include "AudioEngine.h"

USING_NS_CC;
static int _level = 1;

GameScene::GameScene()
	:numOfIce(0), propOfNow(-1),
	isEmptyAnimal(false),isAction(true),
	isTouchAble(true),staSprite(NULL),endSprite(NULL),
	numScore(0),numStep(10),
	toolThree(1),toolFive(1),toolUpdate(1),
	toolHammer(1),toolMagic(1),
	aimCat(12),aimIce(12)/**/ //ok 
{
}

Scene* GameScene::createScene(int level)
{
	_level = level;
	return GameScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//BGM
    if (AudioEngine::getState(bgmId) != AudioEngine::AudioState::PLAYING) {
        bgmId = AudioEngine::play2d("Resources/Audio/PlayBGM.mp3", true);
    }

	//关闭按钮
	auto closeItem = MenuItemImage::create(
		"Resources/PlayGame/ReturnNormal.png",
		"Resources/PlayGame/ReturnSelected.png",
		CC_CALLBACK_1(GameScene::menuReturnCallBack, this));
	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'Resources/CloseNormal.png' and 'Resources/CloseSelected.png'");
	}
	else
	{
		closeItem->setPosition(Vec2(150.0, 150.0));
	}

	//暂停按钮
	auto pauseItem = MenuItemImage::create(
		"Resources/PlayGame/play.jpg",
		"Resources/PlayGame/pause.jpg",
		CC_CALLBACK_1(GameScene::pause, this));
	pauseItem->setPosition(Vec2(GAME_SCREEN_WIDTH -50, GAME_SCREEN_HEIGHT -50));
	
	auto menu = Menu::create(closeItem, pauseItem,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//游戏背景
	auto PlayBG = Sprite::create("Resources/PlayGame/PlayBG.png");
	if (PlayBG == nullptr)
	{
		problemLoading("'Resources/PlayGame/PlayBG.jpg'");
	}
	else
	{
		PlayBG->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
		this->addChild(PlayBG, 0);
	}

	
	//1.运行得到分数和步数的板块   
	getScoreStepGround();
	
	//2.加载字体    
	TTFConfig config("Resources/fonts/yuehei.otf", 50);
	
	//3.分数显示   
	auto labelScore = Label::createWithTTF(config, StringUtils::format("%d", numScore));
	labelScore->setPosition(Vec2(130, 560));
	labelScore->setTag(10);
	labelScore->setColor(Color3B::BLACK);
	this->addChild(labelScore,2);
	
	//4.步数显示  
	auto labelStep = Label::createWithTTF(config, StringUtils::format("%d", numStep));
	labelStep->setPosition(Vec2(130, 425));
	labelStep->setTag(11);
	labelStep->setColor(Color3B(0, 255, 255));
	this->addChild(labelStep, 2);

	//5. 加5步次数显示
	auto labelFive = Label::createWithTTF(config, StringUtils::format("*%d",toolFive));
	labelFive->setPosition(Vec2(GAME_SCREEN_WIDTH - 20, GAME_SCREEN_HEIGHT / 2+5));
	labelFive->setTag(12);
	labelFive->setColor(Color3B::BLACK);
	this->addChild(labelFive, 2);

	//6.加3步次数显示
	auto labelThree = Label::createWithTTF(config, StringUtils::format("*%d", toolThree));
	labelThree->setPosition(Vec2(GAME_SCREEN_WIDTH - 20, GAME_SCREEN_HEIGHT / 2 - 65));
	labelThree->setTag(13);
	labelThree->setColor(Color3B::BLACK);
	this->addChild(labelThree, 2);
	
	//7.打乱小动物顺序次数显示
	auto labelUpdate = Label::createWithTTF(config, StringUtils::format("*%d", toolUpdate));
	labelUpdate->setPosition(Vec2(GAME_SCREEN_WIDTH - 20, GAME_SCREEN_HEIGHT / 2 - 135));
	labelUpdate->setTag(14);
	labelUpdate->setColor(Color3B::BLACK);
	this->addChild(labelUpdate, 2);

	//8.给小动物加特效次数显示
	auto labelMagic = Label::createWithTTF(config, StringUtils::format("*%d", toolMagic));
	labelMagic->setPosition(Vec2(GAME_SCREEN_WIDTH - 20, GAME_SCREEN_HEIGHT / 2 + 75));
	labelMagic->setTag(15);
	labelMagic->setColor(Color3B::BLACK);
	this->addChild(labelMagic, 2);

	//9.敲掉动物次数显示
	auto labelHammer = Label::createWithTTF(config, StringUtils::format("*%d", toolHammer));
	labelHammer->setPosition(Vec2(GAME_SCREEN_WIDTH - 20, GAME_SCREEN_HEIGHT / 2 + 145));
	labelHammer->setTag(16);
	labelHammer->setColor(Color3B::BLACK);
	this->addChild(labelHammer, 2);
	
	//获得地图
	getPositionMap();
	
	//获得“目标”背景
	if (_level == 1)
	{
		auto labelaimCat = Label::createWithTTF(config, StringUtils::format("%d", aimCat));
		labelaimCat->setPosition(Vec2(160, 640));
		labelaimCat->setTag(17);
		labelaimCat->setColor(Color3B::BLACK);
		this->addChild(labelaimCat, 2);
	}
	else if (_level == 2)
	{
		auto labelaimIce = Label::createWithTTF(config, StringUtils::format("%d", aimIce));
		labelaimIce->setPosition(Vec2(160, 640));
		labelaimIce->setTag(17);
		labelaimIce->setColor(Color3B::BLUE);
		this->addChild(labelaimIce, 2);
	}

	initMap();
	
	scheduleUpdate();
	
	//触摸
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	return true;
}

void GameScene::menuReturnCallBack(cocos2d::Ref * pSender)
{
    AudioEngine::stop(bgmId);
	auto nextScene = SelectLevel::create();
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, nextScene));
	MenuItem *item = (MenuItem*)pSender;

}

void GameScene::pause(cocos2d::Ref* pSender)
{
    Director::getInstance()->pushScene(Pause::createScene());
}

void GameScene::buttonCannotUse()
{
	AudioEngine::play2d("Resources/Audio/cannotuse.mp3", false);

	_eventDispatcher->pauseEventListenersForTarget(this, true); //暂停事件监听，实现传说中的模态效果
	Dialog* dialog = Dialog::create();
	dialog->setBackground("Resources/PlayGame/NoRemove.png");
	dialog->addButton("Resources/PlayGame/CloseNormal.png", "Resources/PlayGame/CloseSelected.png", 2,
		Vec2(GAME_SCREEN_WIDTH / 2 + 75, GAME_SCREEN_HEIGHT / 2 + 75));//添加按钮

	dialog->setCallbackFunc(CallFuncN::create(CC_CALLBACK_1(GameScene::buttonCallBack, this)));//设置回调函数
	addChild(dialog, 3);

}

void GameScene::buttonNoAnimals()
{
	AudioEngine::play2d("Resources/Audio/cannotuse.mp3", false);

	_eventDispatcher->pauseEventListenersForTarget(this, true); //暂停事件监听，实现传说中的模态效果
	Dialog* dialog = Dialog::create();
	dialog->setBackground("Resources/PlayGame/NoAnimal.png");
	dialog->addButton("Resources/PlayGame/CloseNormal.png", "Resources/PlayGame/CloseSelected.png", 2,
		Vec2(GAME_SCREEN_WIDTH / 2 + 75, GAME_SCREEN_HEIGHT / 2 + 75));//添加按钮

    dialog->setCallbackFunc(CallFuncN::create(CC_CALLBACK_1(GameScene::buttonCallBack, this)));//设置回调函数
	addChild(dialog, 3);


}

void GameScene::buttonCallBack(Node* pNode)
{
	_eventDispatcher->resumeEventListenersForTarget(this, true);//恢复事件监听
}

void GameScene::creatProp()
{
	propList[RESET] = Prop::create(RESET);
	auto resetItem = MenuItemImage::create(
		PropPics[RESET], SelPropPics[RESET],
		CC_CALLBACK_1(GameScene::menuResetCallback, this));
	resetItem->setPosition(propList[RESET]->getPos());

	propList[ADDTHREE] = Prop::create(ADDTHREE);
	auto addThreeItem = MenuItemImage::create(
		PropPics[ADDTHREE], SelPropPics[ADDTHREE],
		CC_CALLBACK_1(GameScene::menuAddThreeCallback, this));
	addThreeItem->setPosition(propList[ADDTHREE]->getPos());

	propList[ADDFIVE] = Prop::create(ADDFIVE);
	auto addFiveItem = MenuItemImage::create(
		PropPics[ADDFIVE], SelPropPics[ADDFIVE],
		CC_CALLBACK_1(GameScene::menuAddFiveCallback, this));
	addFiveItem->setPosition(propList[ADDFIVE]->getPos());
	
	propList[ADDMAGIC] = Prop::create(ADDMAGIC);
	auto addMagicItem = MenuItemImage::create(
		PropPics[ADDMAGIC], SelPropPics[ADDMAGIC],
		CC_CALLBACK_1(GameScene::menuAddMagicCallback, this));
	addMagicItem->setPosition(propList[ADDMAGIC]->getPos());

	propList[HAMMER] = Prop::create(HAMMER);
	auto hammerItem = MenuItemImage::create(
		PropPics[HAMMER], SelPropPics[HAMMER],
		CC_CALLBACK_1(GameScene::menuHammerCallback, this));
	hammerItem->setPosition(propList[HAMMER]->getPos());

	auto menu = Menu::create(resetItem, addThreeItem,
		addFiveItem, addMagicItem, hammerItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);	
}

void GameScene::menuResetCallback(Ref *pSender)
{
	
	AudioEngine::play2d("Resources/Audio/Click.mp3", false);
	
	if (!isAction)
		return;
	if (staSprite)
	{
		removeSelected();
		staSprite = NULL;
		locList.pop_back();
	}
	if (propList[RESET]->getRemainTimes())
	{
		reset();
		propList[RESET]->setRemainTimes(0);
		toolUpdate--;
	}
	else
	{
		invalidProp();
	}
}

void GameScene::menuAddThreeCallback(Ref *pSender)
{
	
    AudioEngine::play2d("Resources/Audio/Click.mp3", false);
	
	if (!isAction)
		return;
	if (staSprite)
	{
		removeSelected();
		staSprite = NULL;
		locList.pop_back();
	}
	if (propList[ADDTHREE]->getRemainTimes())
	{
		/// 当前步数加3
		//（只有当步数小于3的时候才能使用 才执行下面语句）
		
		if (numStep < 3)
		{
			propList[ADDTHREE]->setRemainTimes(0);
			numStep += 3;
			toolThree--;
		}
		else
			invalidProp();
		
	}
	else
	{
		invalidProp();
	}
}

void GameScene::menuAddFiveCallback(Ref *pSender)
{
	
    AudioEngine::play2d("Resources/Audio/Click.mp3", false);

	
	if (!isAction)
		return;
	if (staSprite)
	{
		removeSelected();
		staSprite = NULL;
		locList.pop_back();
	}
	if (propList[ADDFIVE]->getRemainTimes())
	{
		/// 当前步数加5
		//（只有当步数小于5的时候才能使用 才执行下面语句）
		if (numStep < 5)
		{
			propList[ADDFIVE]->setRemainTimes(0);
			numStep += 5;
			toolFive--;
		}
		else
			invalidProp();
		
	}
	else
	{
		invalidProp();
	}
}

void GameScene::menuAddMagicCallback(Ref *pSender)
{
	
    AudioEngine::play2d("Resources/Audio/Click.mp3", false);

	if (!isAction)
		return;
	if (staSprite)
	{
		
		removeSelected();
		staSprite = NULL;
		locList.pop_back();
	}
	if (propList[ADDMAGIC]->getRemainTimes())
	{
		if (propOfNow == HAMMER)
			propList[HAMMER]->setRemainTimes(1);
		propOfNow = ADDMAGIC;
		propList[ADDMAGIC]->setRemainTimes(0);
		toolMagic--;
	}
	else
	{
		invalidProp();
	}
}

void GameScene::menuHammerCallback(Ref *pSender)
{
	
    AudioEngine::play2d("Resources/Audio/Click.mp3", false);

	if (!isAction)
		return;
	if (staSprite)
	{
		removeSelected();
		staSprite = NULL;
		locList.pop_back();
	}
	if (propList[HAMMER]->getRemainTimes())
	{
		if (propOfNow == ADDMAGIC)
			propList[ADDMAGIC]->setRemainTimes(1);
		propOfNow = HAMMER;
		propList[HAMMER]->setRemainTimes(0);
		toolHammer--;
	}
	else
	{
		invalidProp();
	}
}

bool GameScene::validHammer(Point *point)
{
    AudioEngine::play2d("Resources/Audio/Hammer.mp3", false);

	Block *bloSpr = NULL;
	Rect rect = Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
	for (int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _column; ++j)
		{
			bloSpr = map[i][j];
			if (bloSpr && PosMap.getState(i, j) == ANIMAL)
			{
				rect.origin.x = bloSpr->getPositionX() - (BLOCK_SIZE / 2);
				rect.origin.y = bloSpr->getPositionY() - (BLOCK_SIZE / 2);
				if (rect.containsPoint(*point))
				{
					auto aniSpr = dynamic_cast<Animals*>(bloSpr);
					markRemove(aniSpr);
					removeAnimals();
					return true;
				}
			}
			if (bloSpr && PosMap.getState(i, j) == ICE)
			{
				rect.origin.x = bloSpr->getPositionX() - (BLOCK_SIZE / 2);
				rect.origin.y = bloSpr->getPositionY() - (BLOCK_SIZE / 2);
				if (rect.containsPoint(*point))
				{
					auto iceSpr = dynamic_cast<Ice*>(bloSpr);
					breakIce(iceSpr);
					return true;
				}
			}
		}
	}
	return false;
}

void GameScene::invalidProp()
{
	//弹出提示 ：使用道具无效	
	buttonCannotUse();
	
}

//生成分数和步数所在的背景  ok
void GameScene::getScoreStepGround()
{
	auto SSGround = Sprite::create(StringUtils::format("Resources/PlayGame/Aim_%d.png", _level));
	SSGround->setPosition(Vec2(172, 590));
	this->addChild(SSGround, 1);
}

void GameScene::getPositionMap()
{
	PosMap = MapData(_level);
	_row = PosMap.getRow();
	_column = PosMap.getColumn();
	auto map = Sprite::create(StringUtils::format("Resources/PlayGame/Map_%d.png", _level));
	map->setPosition(Vec2(GAME_SCREEN_WIDTH / 2, GAME_SCREEN_HEIGHT / 2));
	this->addChild(map, 1);

	//弹出提示 本关（包括第一关和第二关)的目标 等
	auto aim = Sprite::create(StringUtils::format("Resources/PlayGame/Use_%d.png", _level));
	aim->setPosition(Vec2(80,640));
	this->addChild(aim, 2);
}

void GameScene::initMap()
{
	getPositionMap();
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == EMPTY)
			{
				map[i][j] = NULL;
				continue;
			}
			if (PosMap.getState(i, j) == ANIMAL)
			{
				createAnimal(i, j);
				continue;
			}
			if (PosMap.getState(i, j) == ICE)
			{
				numOfIce++;
				createIce(i, j);
				continue;
			}
		}
	}
	creatProp();
}

void GameScene::createAnimal(int row, int col)
{
	Point endPosition = PosMap.getPos(row, col);
	Point startPosition = Point(endPosition.x, endPosition.y + GAME_SCREEN_HEIGHT / 2);
	Animals *spr = Animals::create(row, col);
	spr->setPosition(startPosition);
	float duration = (startPosition.y - endPosition.y) / GAME_SCREEN_HEIGHT * 2;
	spr->runAction(MoveTo::create(duration, endPosition));
	this->addChild(spr, 2);

	map[row][col] = spr;

	filledAnimals.push_back(spr);
}

void GameScene::createIce(int row, int col)
{
	Point position = PosMap.getPos(row, col);
	Ice *spr = Ice::create(row, col);
	spr->setPosition(position);
	this->addChild(spr, 2);

	map[row][col] = spr;
}

void GameScene::checkIceBreak(Animals* spr)
{
	
	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	for (int i = 0; i < 4; i++)
	{
		int newR = spr->getRow() + dir[i][0];
		int newC = spr->getColumn() + dir[i][1];
		if (PosMap.isInMap(newR, newC))
		{
			if (PosMap.getState(newR, newC) == ICE)
			{
				Block *bloSpr = NULL;
				bloSpr = map[newR][newC];
				if (!bloSpr)
					continue;
				auto iceSpr = dynamic_cast<Ice*>(bloSpr);
				breakIce(iceSpr);
				
			}
		}
	}
}

void GameScene::breakIce(Ice* spr)
{
    AudioEngine::play2d("Resources/Audio/IceBreak.mp3", false);
	spr->iceBroken();
	PosMap.setAniState(spr->getRow(), spr->getColumn());
	map[spr->getRow()][spr->getColumn()] = NULL;
	
	//消去冰块，再加100分  
	numScore += 100;
	//目标冰块数减一
	aimIce--;
	
}

void GameScene::addSelected()
{
	_selected = Sprite::create("Resources/PlayGame/selected.png");
	if (_selected == nullptr)
	{
		problemLoading("'Resources/selected.jpg'");
	}
	else
	{
		
        AudioEngine::play2d("Resources/Audio/Click.mp3", false);

		_selected->setPosition(Vec2(staSprite->getPositionX(), staSprite->getPositionY()));
		this->addChild(_selected, 1);
		staSprite->setSelected();
	}
}

void GameScene::removeSelected()
{
	staSprite->removeSelected();
	this->removeChild(_selected);
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	if (isTouchAble)
	{
		auto location = touch->getLocation();
		locList.push_back(location);
		getSelAni();
	}
	return isTouchAble;
}

void GameScene::getSelAni()
{
	if (propOfNow)
	{
		if (locList.size() == 1)
		{
			if (propOfNow == ADDMAGIC)
			{
				staSprite = AniOfPoint(&locList[0]);
				if (staSprite)
				{
					staSprite->setState(BOMB);
					staSprite = NULL;
					locList.pop_back();
					propOfNow = -1;
					return;
				}
				else
				{
					invalidProp();
					propList[ADDMAGIC]->setRemainTimes(1);
					staSprite = NULL;
					locList.pop_back();
					propOfNow = -1;
					return;
				}
			}
			if (propOfNow == HAMMER)
			{
				if (validHammer(&locList[0]))
				{
					isEmptyAnimal = true;
					staSprite = NULL;
					locList.pop_back();
					propOfNow = -1;
					return;
				}
				else
				{
					invalidProp();
					propList[HAMMER]->setRemainTimes(1);
					staSprite = NULL;
					locList.pop_back();
					propOfNow = -1;
					return;
				}
			}
		}

	}

	if (locList.size() == 1)
	{
		staSprite = AniOfPoint(&locList[0]);
		if (staSprite)
			addSelected();
		else
		{
			staSprite = NULL;
			locList.pop_back();
		}
		return;
	}
	if (locList.size() == 2)
	{
		staSprite = AniOfPoint(&locList[0]);
		endSprite = AniOfPoint(&locList[1]);
		if (endSprite)
		{
			removeSelected();
			if (canSwap())
			{
				swapSprite();

				staSprite = NULL;
				endSprite = NULL;
				locList.erase(locList.begin(), locList.begin() + 2);
			}
			else
			{
				staSprite = endSprite;
				endSprite = NULL;
				addSelected();
				locList.assign(2, locList[1]);
				locList.pop_back();
			}
		}
		else
		{
			removeSelected();
			staSprite = NULL;
			endSprite = NULL;
			locList.erase(locList.begin(), locList.begin() + 2);
		}
		return;
	}
}

Animals *GameScene::AniOfPoint(Point *point)
{
	Block *bloSpr = NULL;
	Rect rect = Rect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
	for (int i = 0; i < _row; ++i)
	{
		for (int j = 0; j < _column; ++j)
		{
			bloSpr = map[i][j];
			if (bloSpr && PosMap.getState(i, j) == ANIMAL)
			{
				rect.origin.x = bloSpr->getPositionX() - (BLOCK_SIZE / 2);
				rect.origin.y = bloSpr->getPositionY() - (BLOCK_SIZE / 2);
				if (rect.containsPoint(*point))
				{
					auto aniSpr = dynamic_cast<Animals*>(bloSpr);
					return aniSpr;
				}
			}
		}
	}
	return NULL;
}

bool GameScene::canSwap()
{
	if (!staSprite || !endSprite)
		return false;
	int staRow = staSprite->getRow();
	int staCol = staSprite->getColumn();
	int endRow = endSprite->getRow();
	int endCol = endSprite->getColumn();

	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };

	for (int i = 0; i < 4; i++)
	{
		if (endRow == staRow + dir[i][0] && endCol == staCol + dir[i][1])
			return true;
	}
	return false;
}

void GameScene::swapSprite()
{
	
	//交换的时候步数减少  ok
	numStep--;
  
    // 移动中，不允许再次触摸，执行动作设置为true
	isAction = true;
	isTouchAble = false;
	if (!staSprite || !endSprite)
		return;
	
    AudioEngine::play2d("Resources/Audio/SwapAnimal.mp3", false);

	Point posOfSta = staSprite->getPosition();
	Point posOfEnd = endSprite->getPosition();

	float time = 0.1;
	// 在数组中交换位置
	map[staSprite->getRow()][staSprite->getColumn()] = endSprite;
	map[endSprite->getRow()][endSprite->getColumn()] = staSprite;

	int tmpRow = staSprite->getRow();
	int tmpCol = staSprite->getColumn();
	staSprite->setRow(endSprite->getRow());
	staSprite->setColumn(endSprite->getColumn());
	endSprite->setRow(tmpRow);
	endSprite->setColumn(tmpCol);


	if (oneSuperBird())
	{
		staSprite->runAction(MoveTo::create(time, posOfEnd));
		endSprite->runAction(MoveTo::create(time, posOfSta));
		
		if (staSprite->getAniState() == endSprite->getAniState())
		{
			bothSuperBird();
			return;
		}
		if (staSprite->getAniState() == SUPERBIRD)
		{
			
			markRemove(staSprite);
			if (endSprite->getAniState() == COMMON)
				SuperBirdAndCommon(endSprite->getImgIndex());
			if (endSprite->getAniState() == BOMB)
				SuperBirdAndBomb(endSprite->getImgIndex());
			if (endSprite->getAniState() == HORIZONTAL || endSprite->getAniState() == VERTICAL)
				SuperBirdAndHorV(endSprite->getImgIndex());
			return;
		}
		else
		{
			
			markRemove(endSprite);
			if (staSprite->getAniState() == COMMON)
				SuperBirdAndCommon(staSprite->getImgIndex());
			if (staSprite->getAniState() == BOMB)
				SuperBirdAndBomb(staSprite->getImgIndex());
			if (staSprite->getAniState() == HORIZONTAL || staSprite->getAniState() == VERTICAL)
				SuperBirdAndHorV(staSprite->getImgIndex());
			return;
		}
	}

	if (bothUncommon())
	{
		staSprite->runAction(MoveTo::create(time, posOfEnd));
		endSprite->runAction(MoveTo::create(time, posOfSta));
		
		if (staSprite->getAniState() == BOMB || endSprite->getAniState() == BOMB)
		{
			
			markRemove(staSprite);
			markRemove(endSprite);
			if (staSprite->getAniState() == BOMB)
			{
				if (endSprite->getAniState() == HORIZONTAL)
					BombAndHorizonral(staSprite);
				if (endSprite->getAniState() == VERTICAL)
					BombAndVertical(staSprite);
				return;
			}
			else
			{
				if (staSprite->getAniState() == HORIZONTAL)
					BombAndHorizonral(endSprite);
				if (staSprite->getAniState() == VERTICAL)
					BombAndVertical(endSprite);
				return;
			}
		}
		else
		{
			markRemove(staSprite);
			markRemove(endSprite);
			return;
		}

	}

	bool isStaRemove = RemoveAndMark(staSprite);
	bool isEndRemove = RemoveAndMark(endSprite);
	if (isStaRemove || isEndRemove)
	{
		staSprite->runAction(MoveTo::create(time, posOfEnd));
		endSprite->runAction(MoveTo::create(time, posOfSta));
		
		
		removeAnimals();
		return;
	}
	// 在数组中交换位置
	numStep++;
	map[staSprite->getRow()][staSprite->getColumn()] = endSprite;
	map[endSprite->getRow()][endSprite->getColumn()] = staSprite;

	tmpRow = staSprite->getRow();
	tmpCol = staSprite->getColumn();
	staSprite->setRow(endSprite->getRow());
	staSprite->setColumn(endSprite->getColumn());
	endSprite->setRow(tmpRow);
	endSprite->setColumn(tmpCol);

	staSprite->runAction(Sequence::create(
		MoveTo::create(time, posOfEnd),
		MoveTo::create(time, posOfSta),
		NULL));
	endSprite->runAction(Sequence::create(
		MoveTo::create(time, posOfSta),
		MoveTo::create(time, posOfEnd),
		NULL));
}

int GameScene::getRowNum(Animals *spr)
{
	std::vector<int>dCol;
	//spr本身所在列
	dCol.push_back(0);

	int row = spr->getRow();
	int col = spr->getColumn();

	Block *bloSpr = NULL;

	for (int dy = -1; (col + dy) >= 0; dy--)
	{
		if (PosMap.getState(row, col + dy) == ANIMAL)
		{
			bloSpr = map[row][col + dy];
			if (!bloSpr)
				break;
			auto newAnimal = dynamic_cast<Animals*>(bloSpr);
			if (newAnimal
				&& (newAnimal->getImgIndex() == spr->getImgIndex())
				&& !newAnimal->getIgnoreCheck())
				dCol.push_back(dy);
			else
				break;
		}
	}

	for (int dy = 1; (col + dy) < _column; dy++)
	{

		if (PosMap.getState(row, col + dy) == ANIMAL)
		{
			bloSpr = map[row][col + dy];
			if (!bloSpr)
				break;
			auto newAnimal = dynamic_cast<Animals*>(bloSpr);

			if (newAnimal
				&& (newAnimal->getImgIndex() == spr->getImgIndex())
				&& !newAnimal->getIgnoreCheck())
				dCol.push_back(dy);
			else
				break;
		}
	}

	if (dCol.size() >= 3)
	{
		std::vector<int>::iterator it = dCol.begin();
		for (; it != dCol.end(); it++)
		{
			if (PosMap.getState(row, col + *it) == ANIMAL)
			{
				bloSpr = map[row][col + *it];

				if (!bloSpr)
					continue;

				auto markAniSpr = dynamic_cast<Animals*>(bloSpr);
				markRemove(markAniSpr);

			}
		}
	}

	return dCol.size();
}

int GameScene::getColNum(Animals *spr)
{
	std::vector<int>dRow;
	dRow.push_back(0);

	int row = spr->getRow();
	int col = spr->getColumn();

	Block *bloSpr = NULL;

	for (int dx = -1; (row + dx) >= 0; dx--)
	{
		if (PosMap.getState(row + dx, col) == ANIMAL)
		{
			bloSpr = map[row + dx][col];
			if (!bloSpr)
				break;

			auto newAnimal = dynamic_cast<Animals*>(bloSpr);

			if (newAnimal
				&& (newAnimal->getImgIndex() == spr->getImgIndex())
				&& !newAnimal->getIgnoreCheck())
				dRow.push_back(dx);
			else
				break;
		}
	}

	for (int dx = 1; (row + dx) < _row; dx++)
	{
		if (PosMap.getState(row + dx, col) == ANIMAL)
		{
			bloSpr = map[row + dx][col];
			if (!bloSpr)
				break;

			auto newAnimal = dynamic_cast<Animals*>(bloSpr);
			if (newAnimal
				&& (newAnimal->getImgIndex() == spr->getImgIndex())
				&& !newAnimal->getIgnoreCheck())
				dRow.push_back(dx);
			else
				break;
		}
	}

	if (dRow.size() >= 3)
	{
		std::vector<int>::iterator it = dRow.begin();
		for (; it != dRow.end(); it++)
		{

			if (PosMap.getState(row + *it, col) == ANIMAL)
			{
				bloSpr = map[row + *it][col];
				if (!bloSpr)
					continue;
				auto markAniSpr = dynamic_cast<Animals*>(bloSpr);
				markRemove(markAniSpr);
			}
		}
	}

	return dRow.size();
}

bool GameScene::RemoveAndMark(Animals *spr)
{
	int colNum = getColNum(spr);
	int rowNum = getRowNum(spr);

	if (colNum < 3 && rowNum < 3)
		return false;

	if (colNum >= 5 || rowNum >= 5)
	{
		spr->setState(SUPERBIRD);
		spr->setIgnoreCheck(true);
		spr->setIsNeedRemove(false);
		return true;
	}

	if (rowNum >= 4)
	{
		spr->setIgnoreCheck(true);
		spr->setIsNeedRemove(false);
		if (colNum >= 3)
			spr->setState(BOMB);
		else
			spr->setState(HORIZONTAL);
		return true;
	}

	if (colNum >= 4)
	{
		spr->setIgnoreCheck(true);
		spr->setIsNeedRemove(false);
		if (rowNum >= 3)
			spr->setState(BOMB);
		else
			spr->setState(VERTICAL);
		return true;
	}

	if (rowNum >= 3)
	{
		if (colNum >= 3)
		{
			spr->setState(BOMB);
			spr->setIgnoreCheck(true);
			spr->setIsNeedRemove(false);
		}
		return true;
	}

	if (colNum >= 3)
	{
		if (rowNum >= 3)
		{
			spr->setIgnoreCheck(true);
			spr->setIsNeedRemove(false);
			spr->setState(BOMB);
		}
		return true;
	}
	return true;
}

void GameScene::markRemove(Animals* spr)
{
	if (spr->getIsNeedRemove())
		return;

	if (spr->getIgnoreCheck())
		return;

	spr->setIsNeedRemove(true);
}

bool GameScene::bothUncommon()
{
	return (staSprite->getAniState() != COMMON && endSprite->getAniState() != COMMON);
}

bool GameScene::oneSuperBird()
{
	return (staSprite->getAniState() == SUPERBIRD || endSprite->getAniState() == SUPERBIRD);
}

void GameScene::bothSuperBird()
{
	// 全屏消除
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block *tmpBloSpr = map[i][j];
				if (!tmpBloSpr)
					continue;
				auto Anispr = dynamic_cast<Animals*>(tmpBloSpr);
				markRemove(Anispr);
			}
		}
	}
	removeAnimals();
}

void GameScene::SuperBirdAndCommon(int imgIndex)
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block *tmpBloSpr = map[i][j];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				if (!otherAni)
					continue;
				if (otherAni->getImgIndex() == imgIndex)
				{
					otherAni->setIsCommonRemove(false);
					markRemove(otherAni);
				}
			}
		}
	}
	removeAnimals();
}

void GameScene::SuperBirdAndBomb(int imgIndex)
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block *tmpBloSpr = map[i][j];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				if (otherAni->getImgIndex() == imgIndex)
				{
					otherAni->setState(BOMB);
					markRemove(otherAni);
				}
			}
		}
	}
	removeAnimals();
}

void GameScene::SuperBirdAndHorV(int imgIndex)
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block *tmpBloSpr = map[i][j];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				if (otherAni->getImgIndex() == imgIndex)
				{
					// 随机 横消或者竖消
					float _rand = rand() % 2;
					if (_rand < 1)
						otherAni->setState(HORIZONTAL);
					else
						otherAni->setState(VERTICAL);
					markRemove(otherAni);
				}
			}
		}
	}
	removeAnimals();
}

void GameScene::SuperBirdAlone()
{
	float _randImgIndex = rand() % TOTAL_ANI;
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block* tmpBloSpr = map[i][j];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				if (otherAni->getImgIndex() == _randImgIndex)
					markRemove(otherAni);
			}
		}
	}
	removeAnimals();
}

void GameScene::BombAndHorizonral(Animals *spr)
{
	int dir[3] = { -1,0,1 };
	for (int i = 0; i < 3; i++)
	{
		if (spr->getColumn() + dir[i] >= 0 && spr->getColumn() + dir[i] < _column)
		{
			if (PosMap.getState(spr->getRow(), spr->getColumn() + dir[i]) == ANIMAL)
			{
				Block *tmpBloSpr = map[spr->getRow()][spr->getColumn() + dir[i]];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				otherAni->setState(HORIZONTAL);
				markRemove(otherAni);
			}
		}
	}
	removeAnimals();
}

void GameScene::BombAndVertical(Animals *spr)
{
	int dir[3] = { -1,0,1 };
	for (int i = 0; i < 3; i++)
	{
		if (spr->getRow() + dir[i] >= 0 && spr->getRow() + dir[i] < _row)
		{
			if (PosMap.getState(spr->getRow() + dir[i], spr->getColumn()) == ANIMAL)
			{
				Block *tmpBloSpr = map[spr->getRow() + dir[i]][spr->getColumn()];
				if (!tmpBloSpr)
					continue;
				auto otherAni = dynamic_cast<Animals*>(tmpBloSpr);
				otherAni->setState(VERTICAL);
				markRemove(otherAni);
			}
		}
	}
	removeAnimals();
}

void GameScene::fillAnimals()
{
	filledAnimals.clear();
	
	//记录消去的动物的数量 
	int sum = 0;  
	
	isAction = true;

	Block *bloSpr = NULL;
	for (int i = 0; i < _row; i++) 
	{
		int numFilled = 0;
		for (int j = 0; j < _column; j++) 
		{
			//如果为冰块，无需填补冰块以下的动物
			if (PosMap.getState(i, j) == ICE)
			{
				numFilled = 0;
				continue;
			}
			//如果为动物
			if (PosMap.getState(i, j) == ANIMAL)
			{
				bloSpr = map[i][j];
				if (!bloSpr)
					numFilled++;
				else
				{
					if (numFilled > 0)
					{
						int newCol = j - numFilled;
						map[i][newCol] = bloSpr;
						map[i][j] = NULL;
						Point startPosition = bloSpr->getPosition();
						Point endPosition = PosMap.getPos(i, newCol);
						float duration = (startPosition.y - endPosition.y) / GAME_SCREEN_HEIGHT * 2;
						bloSpr->stopAllActions();
						bloSpr->runAction(MoveTo::create(duration, endPosition));

						bloSpr->setColumn(newCol);

						auto fAnimal = dynamic_cast<Animals*>(bloSpr);
						filledAnimals.push_back(fAnimal);
						
                        AudioEngine::play2d("Resources/Audio/Drop.mp3", false);

					}
				}
			}
		}
		sum += numFilled;
		
	}
	
	for (int i = 0; i < _row; i++) 
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				bloSpr = map[i][j];
				if (bloSpr)
					continue;
				else
				{
					int index = 1;
					//判断其上方是否存在冰块
					for (int k = j; k < _column; k++)
					{
						if (PosMap.getState(i, k) == ICE)
						{
							index = 0;
							break;
						}
					}
					if (index)
						createAnimal(i, j);
				}
			}
		}
	}
	
	//消去一个加50分   
	numScore += sum * 50;
	
}

void GameScene::checkAndRemoveAni()
{
	Block *bloSpr = NULL;

	//重置动物的IgnoreCheck为false
	for (int i = 0; i < _row; i++) 
	{
		for (int j = 0; j < _column; j++) 
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				bloSpr = map[i][j];
				if (bloSpr)
				{
					auto aniSpr = dynamic_cast<Animals*>(bloSpr);
					aniSpr->setIgnoreCheck(false);
				}
			}
		}
	}
	
	//检查新移动的动物是否能够消除
	std::vector<Animals*>::iterator it = filledAnimals.begin();
	for (; it != filledAnimals.end(); it++)
	{
		if ((*it)->getIgnoreCheck())
			continue;
		RemoveAndMark(*it);
	}

	removeAnimals();
}

void GameScene::removeAnimals()
{
	isAction = true;

	for (int i = 0; i < _row; i++) 
	{
		for (int j = 0; j < _column; j++) 
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block* bloSpr = map[i][j];
				if (!bloSpr)
					continue;
				auto rAnimal = dynamic_cast<Animals*>(bloSpr);
				if (rAnimal->getIgnoreCheck())
					continue;
				if (rAnimal->getIsNeedRemove())
				{
					isEmptyAnimal = true;

					if (rAnimal->getAniState() == HORIZONTAL)
					{
						removeHorizontal(rAnimal);
						continue;
					}
					if (rAnimal->getAniState() == VERTICAL)
					{
						removeVertical(rAnimal);
						continue;
					}
					if (rAnimal->getAniState() == BOMB)
					{
						removeBomb(rAnimal);
						continue;
					}
					if (rAnimal->getAniState() == SUPERBIRD)
					{
						//因交换魔力鸟而消除
						if (staSprite == rAnimal || endSprite == rAnimal)
						{
							//魔力鸟和普通动物交换
							if (staSprite->getAniState() == COMMON
								|| endSprite->getAniState() == COMMON)
								removeSuperBird(rAnimal);
							else
								dRemoveSuperBird(rAnimal);
						}
						else
						{
							dRemoveSuperBird(rAnimal);
							SuperBirdAlone();
						}
						continue;
					}
					if (rAnimal->getAniState() == COMMON)
					{
						
						if (rAnimal->getImgIndex() == CAT)
						{
							if(aimCat)
								aimCat--;
						}
						//普通的消除
						if (rAnimal->getIsCommonRemove())
							removeCommon(rAnimal);
						//因与魔力鸟交换而消除
						else
							removeSwapCommon(rAnimal);
						continue;
					}
				}
			}
		}
	}
}

void GameScene::removeCommon(Animals* spr)
{
	if (numOfIce)
		checkIceBreak(spr);
	
	map[spr->getRow()][spr->getColumn()] = NULL;

	//动物的动作
	spr->runAction(Sequence::create(
		ScaleTo::create(0.2, 0.0),
		CallFuncN::create(CC_CALLBACK_0(Sprite::removeFromParent, spr)),
		NULL));

	//动物消除的效果 圆圈动作
	auto ExplodeCircle = Sprite::create("Resources/PlayGame/CommonRe.png");
	addChild(ExplodeCircle, 10);
	ExplodeCircle->setPosition(spr->getPosition());
	ExplodeCircle->setScale(0);

	ExplodeCircle->runAction(Sequence::create(ScaleTo::create(0.2, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, ExplodeCircle)),
		NULL));
}

void GameScene::removeSwapCommon(Animals* spr)
{
	
    AudioEngine::play2d("Resources/Audio/SwapSuperBird.mp3", false);
    AudioEngine::play2d("Resources/Audio/Unbelievable.mp3", false);
	
	map[spr->getRow()][spr->getColumn()] = NULL;

	//动物的动作
	spr->runAction(Sequence::create(
		ScaleTo::create(0.2, 0.0),
		CallFuncN::create(CC_CALLBACK_0(Sprite::removeFromParent, spr)),
		NULL));

	//动物消除的效果 圆圈动作
	auto ExplodeCircle = Sprite::create("Resources/PlayGame/SwapCommonRe.png");
	addChild(ExplodeCircle, 10);
	ExplodeCircle->setPosition(spr->getPosition());
	ExplodeCircle->setScale(0);

	ExplodeCircle->runAction(Sequence::create(ScaleTo::create(0.2, 1.0),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, ExplodeCircle)),
		NULL));
}

void GameScene::removeHorizontal(Animals *spr)
{
	
    AudioEngine::play2d("Resources/Audio/ExplodeB.mp3", false);
    AudioEngine::play2d("Resources/Audio/Excellent.mp3", false);

	
	auto ExplodeH = Sprite::create("Resources/PlayGame/HorizontalRe.png");
	addChild(ExplodeH, 10);
	ExplodeH->setPosition(GAME_SCREEN_WIDTH / 2, PosMap.getPos(spr->getRow(), spr->getColumn()).y);

	ExplodeH->setScaleY(0);
	
	ExplodeH->runAction(Sequence::create(ScaleTo::create(0.3, 1, 0.8),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, ExplodeH)),
		NULL));

	for (int r = 0; r < _row; r++)
	{
		Block *tmpBloSpr = map[r][spr->getColumn()];
		if (!tmpBloSpr)
			continue;
		if (PosMap.getState(r, spr->getColumn()) == ANIMAL)
		{
			auto tmp = dynamic_cast<Animals*>(tmpBloSpr);
			markRemove(tmp);
			continue;
		}
		if (PosMap.getState(r, spr->getColumn()) == ICE)
		{
			auto iceSpr = dynamic_cast<Ice*>(tmpBloSpr);
			breakIce(iceSpr);
		}
	}
	spr->setState(COMMON);
	removeAnimals();
}

void GameScene::removeVertical(Animals *spr)
{
    AudioEngine::play2d("Resources/Audio/ExplodeB.mp3", false);
    AudioEngine::play2d("Resources/Audio/Great.mp3", false);

	
	auto ExplodeV = Sprite::create("Resources/PlayGame/VerticalRe.png");
	addChild(ExplodeV, 10);
	ExplodeV->setPosition(PosMap.getPos(spr->getRow(), spr->getColumn()).x, GAME_SCREEN_HEIGHT / 2);
	ExplodeV->setScaleX(0);

	ExplodeV->runAction(Sequence::create(ScaleTo::create(0.3, 0.8, 1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, ExplodeV)),
		NULL));

	for (int c = 0; c < _column; c++)
	{
		Block *tmpBloSpr = map[spr->getRow()][c];
		if (!tmpBloSpr)
			continue;
		if (PosMap.getState(spr->getRow(), c) == ANIMAL)
		{
			auto tmp = dynamic_cast<Animals*>(tmpBloSpr);
			markRemove(tmp);
			continue;
		}
		if (PosMap.getState(spr->getRow(), c) == ICE)
		{
			auto iceSpr = dynamic_cast<Ice*>(tmpBloSpr);
			breakIce(iceSpr);
		}
	}
	spr->setState(COMMON);
	removeAnimals();
}

void GameScene::removeBomb(Animals *spr)
{
	
	AudioEngine::play2d("Resources/Audio/RemoveBomb.mp3", false);
	AudioEngine::play2d("Resources/Audio/Good.mp3", false);
    
	int dir[12][2] =
	{ {2,0},{0,-2},{-2,0},{0,2},
	{1,0},{0,-1},{-1,0},{0,1},
	{1,-1},{-1,-1},{-1,1},{1,1} };

	for (int i = 0; i < 12; i++)
	{
		int newR = spr->getRow() + dir[i][0];
		int newC = spr->getColumn() + dir[i][1];
		if (PosMap.isInMap(newR, newC))
		{
			Block *tmpBloSpr = map[newR][newC];
			if (!tmpBloSpr)
				continue;
			if (PosMap.getState(newR, newC) == ANIMAL)
			{
				auto tmp = dynamic_cast<Animals*>(tmpBloSpr);
				markRemove(tmp);
				continue;
			}
			if (PosMap.getState(newR, newC) == ICE)
			{
				auto iceSpr = dynamic_cast<Ice*>(tmpBloSpr);
				breakIce(iceSpr);
				continue;
			}
		}
	}
	spr->setState(COMMON);
	removeAnimals();
}

void GameScene::removeSuperBird(Animals *spr)
{

	AudioEngine::play2d("Resources/Audio/remove.mp3", false);
	AudioEngine::play2d("Resources/Audio/Great.mp3", false);
	
	map[spr->getRow()][spr->getColumn()] = NULL;
	
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation("SuperBird");
	auto animate = Animate::create(animation);

	spr->runAction(Sequence::create(animate,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, spr)),
		NULL));
}

void GameScene::dRemoveSuperBird(Animals* spr)
{
	
	AudioEngine::play2d("Resources/Audio/dremove.mp3", false);
	AudioEngine::play2d("Resources/Audio/Unbelievable.mp3", false);
	
	map[spr->getRow()][spr->getColumn()] = NULL;
	auto animation = Animation::create();
	animation = AnimationCache::getInstance()->getAnimation("DSuperBird");
	auto animate = Animate::create(animation);

	spr->runAction(Sequence::create(animate,
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, spr)),
		NULL));
}

bool GameScene::haveStep()const
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block* bloSpr = map[i][j];
				auto aniSpr = dynamic_cast<Animals*>(bloSpr);
				if (aniSpr->getAniState() == SUPERBIRD)
					return true;
				if (aniSpr->getAniState() != COMMON)
				{
					if (checkhaveSMove(i, j))
						return true;
				}
			}
		}
	}

	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				Block *bloSpr = map[i][j];
				auto aniSpr = dynamic_cast<Animals*>(bloSpr);
				int imgIndex = aniSpr->getImgIndex();
				if (checkHaveMove(imgIndex, i + 1, j, i - 1, j - 1, i - 1, j + 1))
					return true;
				if (checkHaveMove(imgIndex, i, j - 1, i - 1, j + 1, i + 1, j + 1))
					return true;
				if (checkHaveMove(imgIndex, i - 1, j, i + 1, j + 1, i + 1, j - 1))
					return true;
				if (checkHaveMove(imgIndex, i, j + 1, i + 1, j - 1, i - 1, j - 1))
					return true;
			}
		}
	}
	return false;
}

bool GameScene::checkhaveSMove(int r, int c)const
{
	int dir[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };
	for (int i = 0; i < 4; i++)
	{
		int newR = r + dir[i][0];
		int newC = c + dir[i][1];
		if (PosMap.isInMap(newR, newC) && PosMap.getState(newR, newC) == ANIMAL)
		{
			Block* bloSpr = map[newR][newC];
			auto aniSpr = dynamic_cast<Animals*>(bloSpr);
			if (aniSpr->getAniState() != COMMON)
				return true;

		}
	}
	return false;
}

bool GameScene::checkHaveMove(int imgIndex,int r1, int c1, int r2, int c2, int r3, int c3)const
{
	if (PosMap.isInMap(r1, c1) && PosMap.getState(r1, c1) == ANIMAL)
	{
		Block *bloSpr1 = map[r1][c1];
		auto aniSpr1 = dynamic_cast<Animals*>(bloSpr1);
		if (aniSpr1->getImgIndex() == imgIndex)
		{
			if (PosMap.isInMap(r2, c2) && PosMap.getState(r2, c2) == ANIMAL)
			{
				Block *bloSpr2 = map[r2][c2];
				auto aniSpr2 = dynamic_cast<Animals*>(bloSpr2);
				if (aniSpr2->getImgIndex() == imgIndex)
					return true;
			}
			if (PosMap.isInMap(r3, c3) && PosMap.getState(r3, c3) == ANIMAL)
			{
				Block *bloSpr3 = map[r3][c3];
				auto aniSpr3 = dynamic_cast<Animals*>(bloSpr3);
				if (aniSpr3->getImgIndex() == imgIndex)
					return true;
			}
		}
	}

	if (PosMap.isInMap(r2, c2) && PosMap.getState(r2, c2) == ANIMAL)
	{
		Block *bloSpr2 = map[r2][c2];
		auto aniSpr2 = dynamic_cast<Animals*>(bloSpr2);
		if (aniSpr2->getImgIndex() == imgIndex)
		{
			if (PosMap.isInMap(r3, c3) && PosMap.getState(r3, c3) == ANIMAL)
			{
				Block *bloSpr3 = map[r3][c3];
				auto aniSpr3 = dynamic_cast<Animals*>(bloSpr3);
				if (aniSpr3->getImgIndex() == imgIndex)
					return true;
			}
		}
	}

	return false;
}

void GameScene::reset()
{
	int refill[MAX_LENGTH][MAX_LENGTH] = { 0 };
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _column; j++)
		{
			if (PosMap.getState(i, j) == ANIMAL)
			{
				int newR = rand() % _row;
				int newC = rand() % _column;
				if (PosMap.getState(newR, newC) == ANIMAL
					&& (!refill[newR][newC]))
				{
					refill[i][j] = 1;
					refill[newR][newC] = 1;

					Block* oriSpr = map[i][j];
					Block* newSpr = map[newR][newC];

					float time = 0.1;
					Point posOfOri = PosMap.getPos(i, j);
					Point posOfNew = PosMap.getPos(newR, newC);

					oriSpr->stopAllActions();
					oriSpr->runAction(MoveTo::create(time, posOfNew));
					newSpr->stopAllActions();
					newSpr->runAction(MoveTo::create(time, posOfOri));

					map[oriSpr->getRow()][oriSpr->getColumn()] = newSpr;
					map[newSpr->getRow()][newSpr->getColumn()] = oriSpr;

					int oriRow = oriSpr->getRow();
					int oriCol = oriSpr->getColumn();
					oriSpr->setRow(newSpr->getRow());
					oriSpr->setColumn(newSpr->getColumn());
					newSpr->setRow(oriRow);
					newSpr->setColumn(oriCol);

				}
			}
		}
	}
}

// 更新函数
void GameScene::update(float t)
{
	//判断是否在执行动作
	if (isAction) 
	{
		//先初步设置为false
		isAction = false;
		for (int i = 0; i < _row; ++i)
		{
			for (int j = 0; j < _column; ++j)
			{
				if (PosMap.getState(i, j) == ANIMAL)
				{
					Block *tmpBloSpr = map[i][j];
					if (!tmpBloSpr)
						continue;
					auto tmpAniSpr = dynamic_cast<Animals*>(tmpBloSpr);
					if (staSprite)
						continue;
					if (tmpAniSpr
						&& tmpAniSpr->getAniState() == COMMON
						&& tmpAniSpr->getNumberOfRunningActions() > 0)
					{
						isAction = true;
						break;
					}
				}
			}
			if (isAction)
				break;
		}
	}

	isTouchAble = !isAction;

	if (isTouchAble)
	{
		if (isEmptyAnimal)
		{
			staSprite = NULL;
			endSprite = NULL;
			fillAnimals();
			isEmptyAnimal = false;

		}
		else
			checkAndRemoveAni();
	}

	if (isTouchAble && (!isEmptyAnimal))
	{
		//在reset前面还有弹出来窗口 提示说 没有可消除的小动物了
		if (!haveStep())
		{
			buttonNoAnimals();
			reset();
		}
			
	}

	//分数变化 
	Label* labelScore = (Label*)this->getChildByTag(10);
	labelScore->setString(StringUtils::format("%d", numScore));

	//步数变化 
	Label* labelStep = (Label*)this->getChildByTag(11);
	labelStep->setString(StringUtils::format("%d", numStep));
	
	//加5的道具数变化
	Label* labelFive = (Label*)this->getChildByTag(12);
	labelFive->setString(StringUtils::format("*%d", propList[ADDFIVE]->getRemainTimes()));
	
	//加3的道具数变化
	Label* labelThree = (Label*)this->getChildByTag(13);
	labelThree->setString(StringUtils::format("*%d", propList[ADDTHREE]->getRemainTimes()));
	
	//打乱的道具数变化
	Label* labelUpdate = (Label*)this->getChildByTag(14);
	labelUpdate->setString(StringUtils::format("*%d", propList[RESET]->getRemainTimes()));
	
	//加特效的道具数变化
	Label* labelMagic = (Label*)this->getChildByTag(15);
	labelMagic->setString(StringUtils::format("*%d", propList[ADDMAGIC]->getRemainTimes()));
	
	//锤子的数量变化
	Label* labelHammer = (Label*)this->getChildByTag(16);
	labelHammer->setString(StringUtils::format("*%d", propList[HAMMER]->getRemainTimes()));

	//目标数
	if (_level == 1)
	{
		Label* labelaimCat = (Label*)this->getChildByTag(17);
		labelaimCat->setString(StringUtils::format("%d", aimCat));
	}
	else if (_level == 2)
	{
		Label* labelaimIce = (Label*)this->getChildByTag(17);
		labelaimIce->setString(StringUtils::format("%d", aimIce));

	}
	
	if (isTouchAble)
	{
		//无步数，则游戏结束
		if (numStep <= 0)
		{
			gameOver(numStep);
		}
	}
	
}

void GameScene::gameOver(float dt)
{
    AudioEngine::stopAll();
	auto scene = Scene::create();
	auto layer = GameOver::create();

	//传递分数
	layer->getScore(numScore);
	
	//传递游戏结果
	if (_level == 2)
	{
		layer->getAim(aimIce);
	}
	else if (_level == 1)
	{
		layer->getAim(aimCat);
	}
	
	scene->addChild(layer);
	
	Director::getInstance()->replaceScene(
		TransitionSlideInT::create(1.0f / 60, scene));
	
}
