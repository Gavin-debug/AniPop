 #pragma once
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Data/Constant.h"

USING_NS_CC_EXT;

class Pause :public cocos2d::Layer
{
public:
	
	static cocos2d::Scene* createScene();
	
	virtual bool init();

	//ÍË³ö
	void menuBackCallback(cocos2d::Ref* pSender);

	//ÖØÍæ
	void menuRestartCallback(cocos2d::Ref* pSender);

	//¼ÌÐø
	void menuContinueCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(Pause);
};

#endif
