#pragma once
#ifndef _MAP_DATA_H__
#define _MAP_DATA_H__

#include"cocos2d.h"

USING_NS_CC;

class MapData
{

private:
	
	int _row;
	
	int _column;
	
	std::vector<Point>_aniPos;
	
	std::vector<int>_staMap;

public:

	MapData() {}
	
	MapData(int level);
	
	Point getPos(int x, int y)const;
	
	int getRow()const;
	
	int getColumn()const;
	
	int getState(int x, int y)const;
	
	void setAniState(int x, int y);
	
	bool isInMap(int x, int y)const;
};

#endif   // !_MAP_DATA_H__
