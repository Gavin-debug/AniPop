#pragma once
#ifndef __SETVOLUME_SCENE_H__
#define __SETVOLUME_SCENE_H__


#include "cocos2d.h"
#include "Data/Constant.h"

class SetVolume : public cocos2d::Scene
{
public:
	
	static cocos2d::Scene* createScene();

	virtual bool init();

	//·µ»Ø
	void menuReturnCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(SetVolume);
};

#endif 