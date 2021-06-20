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

	//��֡����
	void update(float dt);
	
	//��Ϸ����
	void gameOver(float dt);

	//��Ϸ��ͣ
	void pause(cocos2d::Ref* pSender);
	
	void menuReturnCallBack(cocos2d::Ref* pSender);

	void buttonCallBack(Node* pNode);
	
	//�������޷�ʹ��
	void buttonCannotUse();

	//������û�п����Ķ���
	void buttonNoAnimals();

	CREATE_FUNC(GameScene);
	
private:

	int _row;
	int _column;
	//������
	int numOfIce;
	//������
	int toolFive;
	int toolThree;
	int toolUpdate;
	int toolHammer;
	int toolMagic;
	//����
	int numScore;
	//����
	int numStep;
	//Ŀ������������
	int aimCat;
	int aimIce;
	int propOfNow;
    
	//�Ƿ���ִ�ж���
	bool isAction;
	//�Ƿ��ܹ�����
	bool isTouchAble;
	//�Ƿ��п�ȱ��Ҫ�
	bool isEmptyAnimal;

	//������
	std::vector<Vec2>locList;

	Block* map[MAX_LENGTH][MAX_LENGTH];
	
	//�������
	Prop* propList[TOTAL_PRO];
	
	//��������
	void creatProp();
	//����
	void menuResetCallback(Ref *pSender);
	//+3
	void menuAddThreeCallback(Ref *pSender);
	//+5
	void menuAddFiveCallback(Ref *pSender);
	//���������Ч
	void menuAddMagicCallback(Ref *pSender);
	//�õ�����
	void menuHammerCallback(Ref *pSender);
	bool validHammer(Point *point);
	//�޷�ʹ��
	void invalidProp();

	//��Ŀ�ꡱ����
	void getScoreStepGround();
	
	//��ͼ���
	MapData PosMap;
	void getPositionMap();
	void initMap();

	//���춯��ͱ���
	void createAnimal(int row, int col);
	void createIce(int row, int col);
	void checkIceBreak(Animals *spr);
	void breakIce(Ice* spr);

	//ִ�н�����ǰ����������
	Animals* staSprite;
	Animals* endSprite;

	//ѡ���
	Sprite* _selected;
	void addSelected();
	void removeSelected();

	//��ȡѡ��Ķ���
	void getSelAni();
	//��ȡ����λ�õĶ���
	Animals *AniOfPoint(Point *point);

	//���ƶ��Ķ���
	std::vector<Animals*>filledAnimals;

	//��λ�����ж��ܷ񽻻�
	bool canSwap();
	// ��������
	void swapSprite();

	//����ĳһ�����ͬ������ͬ����ĸ���
	int getRowNum(Animals *spr);
	//����ĳһ�����ͬ������ͬ����ĸ���
	int getColNum(Animals *spr);

	//�ж��ܷ����� ���� �������
	bool RemoveAndMark(Animals *spr);

	//����������Ķ���
	void markRemove(Animals* spr);

	//ִ�н������������������Ч
	bool bothUncommon();
	//ִ�н������������ﵱ����һ��Ϊħ����
	bool oneSuperBird();

	//��Ч
	void bothSuperBird();
	void SuperBirdAndCommon(int imgIndex);
	void SuperBirdAndBomb(int imgIndex);
	void SuperBirdAndHorV(int imgIndex);

	void SuperBirdAlone();

	void BombAndHorizonral(Animals *spr);
	void BombAndVertical(Animals *spr);

	void fillAnimals();

	// ��鲢��������
	void checkAndRemoveAni();
	

	// ��������
	void removeAnimals();
	void removeCommon(Animals* spr);
	void removeSwapCommon(Animals* spr);
	void removeHorizontal(Animals *spr);
	void removeVertical(Animals *spr);
	void removeBomb(Animals* spr);
	//������֮��������Ķ����ħ����
	void removeSuperBird(Animals* spr);
	//�������ĳһ�ֶ���/ȫ��������ħ����
	void dRemoveSuperBird(Animals* spr);


	//�ж��Ƿ���ڿ��в�
	bool haveStep()const;
	bool checkhaveSMove(int r, int c)const;
	bool checkHaveMove(int imgIndex, int r1, int c1, int r2, int c2, int r3, int c3)const;
	void reset();
};

#endif // !_GAME_SCENE_H__
