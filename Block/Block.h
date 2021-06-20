#pragma once
#ifndef _BLOCK_H__
#define _BLOCK_H__

#include"cocos2d.h"
#include"Data/Constant.h"

USING_NS_CC;

class Block :public cocos2d::Sprite
{
public:

	Block():_row(0), _column(0)
	{
	}
	
	virtual bool init(int row, int column)
	{
		return false;
	}
	
	CC_SYNTHESIZE(int, _row, Row);
	
	CC_SYNTHESIZE(int, _column, Column);
};
#endif // !_BLOCK_H__
