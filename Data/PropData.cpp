#include"PropData.h"

Prop *Prop::create(PropType type)
{
    Prop *tmpProp = new(std::nothrow) Prop;
    tmpProp->_remainTimes = 1;
    tmpProp->_type = type;
    return tmpProp;
}

Point Prop::getPos()
{
    switch (_type)
    {
    case 0:
        return Point(1205, 250);
        break;
    case 1:
        return Point(1205, 325);
        break;
    case 2:
        return Point(1205, 400);
        break;
    case 3:
        return Point(1205, 475);
        break;
    case 4:
        return Point(1205, 550);
        break;
    default:
        break;
    }
    return NULL;
}
