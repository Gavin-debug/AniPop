#pragma once
#ifndef _ICE_H__
#define _ICE_H__

#include"cocos2d.h"
#include"Block.h"
#include"Data/Constant.h"

USING_NS_CC;

class Ice :public Block
{
public:
	
	static Ice* create(int row, int column);

	virtual bool init(int row, int column);
	
	void iceBroken();
};

#endif // !_ICE_H_
