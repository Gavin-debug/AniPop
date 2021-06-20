#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class LoadingScene :public cocos2d::Scene
{

public:
	
	static cocos2d::Scene* createScene();
	
	virtual void onEnterTransitionDidFinish();
	
	virtual bool init();
	
	CREATE_FUNC(LoadingScene);

private:
	
	void LoadingAnimation();
	
	void tranScene();
};

#endif // !__LOADING_SCENE_H__