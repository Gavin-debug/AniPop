#pragma once
#ifndef _ANIMALS_H__
#define _ANIMALS_H__

#include"cocos2d.h"
#include"Block.h"
#include"Data/Constant.h"

USING_NS_CC;

class Animals :public Block
{
public:
	
	Animals();
	
	static Animals* create(int row, int column);

	virtual bool init(int row, int column);
	
	CC_SYNTHESIZE(int, _imgIndex, ImgIndex);
	CC_SYNTHESIZE(bool, _isNeedRemove, IsNeedRemove);
	CC_SYNTHESIZE(bool, _isCommonRemove, IsCommonRemove);
	CC_SYNTHESIZE(bool, _ignoreCheck, IgnoreCheck);
	CC_SYNTHESIZE_READONLY(AnimalsState, _aniState, AniState);


	void setSelected();
	
	void removeSelected();


	void setState(AnimalsState aniState);
	
	void setVertical();
	
	void setHorizontal();
	
	void setBomb();
	
	void setSuperBird();
};
#endif // !_ANIMALS_H_
