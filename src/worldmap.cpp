#include "rune/worldmap.h"

rune::WorldMap::WorldMap()
{
}

rune::WorldMap::~WorldMap()
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

    // check if coordinate was excluded
    if(excluded.contains(x))
        if(excluded[x].contains(y))
            return false;

    return true;
}

void rune::WorldMap::exclude(quint64 x, quint64 y)
{
    if(!isPointOnMap(x, y))
        return; // already not part of the map

    if(!excluded.contains(x))
    {
        // add x coordinate if not already excluding any y coordinates for it
        QList<quint64> l;
        excluded[x] = l;
    }

    QList<quint64> yCoordinates = excluded[x];
    if(yCoordinates.contains(y))
        return; // y-coordinate already excluded

    qDebug() << "excluding " << x << ":" << y;
    yCoordinates.append(y);
    excluded[x] = yCoordinates;
    return;
}
