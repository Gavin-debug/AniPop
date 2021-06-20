#pragma once
#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

#include "cocos2d.h"
#include "Data/Constant.h"

USING_NS_CC;

class GameOver :public cocos2d::Scene
{
public:

	static Scene* createScene();

	bool init();

	void getScore(int score);

	void getAim(int aim);

	//��Ϸ���������¿�ʼ
	void menuReplayCallback(Ref* pSender);

	//�˳���Ϸ���ص��ؿ�����
	void menuReturnCallback(Ref* pSender);

	CREATE_FUNC(GameOver);
};
#endif