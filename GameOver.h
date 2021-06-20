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

	//游戏结束后重新开始
	void menuReplayCallback(Ref* pSender);

	//退出游戏，回到关卡界面
	void menuReturnCallback(Ref* pSender);

	CREATE_FUNC(GameOver);
};
#endif