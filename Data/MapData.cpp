#include"Constant.h"
#include"MapData.h"

MapData::MapData(int level)
{
    switch (level)
    {
    case 1:
    {
        _row = 6;
        _column = 6;
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _column; j++)
            {
                int x = 90 * i - 225 + GAME_SCREEN_WIDTH / 2;
                int y = 90 * j - 225 + GAME_SCREEN_HEIGHT / 2;
                Point temp = Point(x, y);
                //Anipos[k]  k=i*_row+j
                _aniPos.push_back(temp);
                _staMap.push_back(ANIMAL);
            }
        }
    }
    break;
    case 2:
    {
        _row = 7;
        _column = 7;
        for (int i = 0; i < _row; i++)
        {
            for (int j = 0; j < _column; j++)
            {
                int x = 90 * i - 270 + GAME_SCREEN_WIDTH / 2;
                int y = 90 * j - 270 + GAME_SCREEN_HEIGHT / 2;
                Point temp = Point(x, y);
                _aniPos.push_back(temp);

                if ((i == 0 && j == 0) || (i == 0 && j == 6) ||
                    (i == 6 && j == 0) || (i == 6 && j == 6))
                {
                    _staMap.push_back(EMPTY);
                    continue;
                }
                if (j < 2)
                    _staMap.push_back(ICE);
                else
                    _staMap.push_back(ANIMAL);
            }
        }
    }
    break;
    default:
        break;
    }
}

Point MapData::getPos(int x, int y)const
{
    return _aniPos[x*_row + y];
}

int MapData::getState(int x, int y)const
{
    return _staMap[x*_row + y];
}

void MapData::setAniState(int x, int y)
{
    _staMap[x*_row + y] = ANIMAL;
}

int MapData::getRow()const
{
    return _row;
}

int MapData::getColumn()const
{
    return _column;
}

bool MapData::isInMap(int x, int y)const
{
    if (x >= 0 && x < _row && y >= 0 && y < _column)
    {
        if (getState(x, y) == EMPTY)
            return false;
        else
            return true;
    }
    else
        return false;
}
