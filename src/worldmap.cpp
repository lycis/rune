#include "rune/worldmap.h"

rune::WorldMap::WorldMap()
{
}

quint64 rune::WorldMap::width()
{
    return _width;
}

quint64 rune::WorldMap::height()
{
    return _height;
}

void rune::WorldMap::setWidth(quint64 width)
{
    _width = width;
}

void rune::WorldMap::setHeight(quint64 height)
{
    _height = height;
}

bool rune::WorldMap::isPointOnMap(quint64 x, quint64 y)
{
    // checking for zero is not necessary as height and width are unsigned

    if(x >= _width)
        return false;

    if(y >= _height)
        return false;

    return true;
}
