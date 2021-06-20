#pragma once
#ifndef _GAME_SCENE_H__
#define _GAME_SCENE_H__

#include"cocos2d.h"
#include"Data/Constant.h"
#include"Data/MapData.h"
#include"Data/PropData.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Block;
class Animals;
class Ice;
class MapData;
class Prop;

class GameScene :public cocos2d::Scene
{
public:
	
	GameScene();
	
	static cocos2d::Scene* createScene(int level);

	bool init();
	
	bool onTouchBegan(Touch *touch, Event *unused);

	//逐帧更新
	void update(float dt);
	
	//游戏结束
	void gameOver(float dt);

	//游戏暂停
	void pause(cocos2d::Ref* pSender);
	
	void menuReturnCallBack(cocos2d::Ref* pSender);

	void buttonCallBack(Node* pNode);
	
	//弹窗：无法使用
	void buttonCannotUse();

	//弹窗：没有可消的动物
	void buttonNoAnimals();

	CREATE_FUNC(GameScene);
	
private:

	int _row;
	int _column;
	//冰块数
	int numOfIce;
	//工具数
	int toolFive;
	int toolThree;
	int toolUpdate;
	int toolHammer;
	int toolMagic;
	//分数
	int numScore;
	//步数
	int numStep;
	//目标消除的数量
	int aimCat;
	int aimIce;
	int propOfNow;
    
	//是否在执行动作
	bool isAction;
	//是否能够触摸
	bool isTouchAble;
	//是否有空缺需要填补
	bool isEmptyAnimal;

	//触摸点
	std::vector<Vec2>locList;

	Block* map[MAX_LENGTH][MAX_LENGTH];
	
	//道具相关
	Prop* propList[TOTAL_PRO];
	
	//创建道具
	void creatProp();
	//重置
	void menuResetCallback(Ref *pSender);
	//+3
	void menuAddThreeCallback(Ref *pSender);
	//+5
	void menuAddFiveCallback(Ref *pSender);
	//给动物加特效
	void menuAddMagicCallback(Ref *pSender);
	//敲掉动物
	void menuHammerCallback(Ref *pSender);
	bool validHammer(Point *point);
	//无法使用
	void invalidProp();

	//“目标”背景
	void getScoreStepGround();
	
	//地图相关
	MapData PosMap;
	void getPositionMap();
	void initMap();

	//创造动物和冰块
	void createAnimal(int row, int col);
	void createIce(int row, int col);
	void checkIceBreak(Animals *spr);
	void breakIce(Ice* spr);

	//执行交换的前后两个动物
	Animals* staSprite;
	Animals* endSprite;

	//选择框
	Sprite* _selected;
	void addSelected();
	void removeSelected();

	//获取选择的动物
	void getSelAni();
	//获取具体位置的动物
	Animals *AniOfPoint(Point *point);

	//新移动的动物
	std::vector<Animals*>filledAnimals;

	//在位置上判断能否交换
	bool canSwap();
	// 交换精灵
	void swapSprite();

	//计算某一动物的同行中相同种类的个数
	int getRowNum(Animals *spr);
	//计算某一动物的同列中相同种类的个数
	int getColNum(Animals *spr);

	//判断能否消除 若能 标记下来
	bool RemoveAndMark(Animals *spr);

	//标记能消除的动物
	void markRemove(Animals* spr);

	//执行交换的两个动物均有特效
	bool bothUncommon();
	//执行交换的两个动物当中有一个为魔力鸟
	bool oneSuperBird();

	//特效
	void bothSuperBird();
	void SuperBirdAndCommon(int imgIndex);
	void SuperBirdAndBomb(int imgIndex);
	void SuperBirdAndHorV(int imgIndex);

	void SuperBirdAlone();

	void BombAndHorizonral(Animals *spr);
	void BombAndVertical(Animals *spr);

	void fillAnimals();

	// 检查并消除精灵
	void checkAndRemoveAni();
	

	// 消除精灵
	void removeAnimals();
	void removeCommon(Animals* spr);
	void removeSwapCommon(Animals* spr);
	void removeHorizontal(Animals *spr);
	void removeVertical(Animals *spr);
	void removeBomb(Animals* spr);
	//消除与之交换种类的动物的魔力鸟
	void removeSuperBird(Animals* spr);
	//随机消除某一种动物/全屏消除的魔力鸟
	void dRemoveSuperBird(Animals* spr);


	//判断是否存在可行步
	bool haveStep()const;
	bool checkhaveSMove(int r, int c)const;
	bool checkHaveMove(int imgIndex, int r1, int c1, int r2, int c2, int r3, int c3)const;
	void reset();
};

#endif // !_GAME_SCENE_H__
