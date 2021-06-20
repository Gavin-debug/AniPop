#pragma once
#ifndef _PROP_DATA_H__
#define _PROP_DATA_H__

#include"cocos2d.h"
#include"Constant.h"

USING_NS_CC;

class Prop 
{
public:
	
	static Prop* create(PropType type);

	CC_SYNTHESIZE(int, _remainTimes, RemainTimes);
	
	CC_SYNTHESIZE_READONLY(PropType, _type, Type);

	Point getPos();

};
#endif // !_PROP_DATA_H__
