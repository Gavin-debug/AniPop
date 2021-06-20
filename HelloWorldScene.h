#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Data/Constant.h"

class HelloWorld : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	void menuStartCallBack(cocos2d::Ref* pSender);
   
    void menuCloseCallBack(cocos2d::Ref* pSender);
	
    void menuSetCallback(cocos2d::Ref* pSender);
	
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

};

#endif // __HELLOWORLD_SCENE_H__
