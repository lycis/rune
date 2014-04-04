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
    if(!excluded.contains(x))
    {
        // add x coordinate if not already excluding any y coordinates for it
        QList<quint64> l;
        excluded[x] = l;
    }

    QList<quint64> yCoordinates = excluded[x];
    if(yCoordinates.contains(y))
        return; // y-coordinate already excluded

    yCoordinates.append(y);
    excluded[x] = yCoordinates;
    return;
}

void rune::WorldMap::exclude(rune::map_coordinate mc)
{
    exclude(mc.x, mc.y);
    return;
}

void rune::WorldMap::excludeCircle(quint64 x, qint64 y, quint64 radius)
{
    QList<rune::map_coordinate> coords = getCoordinateCircle(x, y, radius);
    for(QList<rune::map_coordinate>::iterator it = coords.begin(); it!=coords.end(); ++it)
    {
        exclude(*it);
    }
    return;
}

void rune::WorldMap::include(quint64 x, quint64 y)
{
    if(isPointOnMap(x, y))
        return; // already part of the map

    if(_width < x)
    {
        // this column is not on the map yet -> exclude all added columns
        for(quint64 col = _width; col <= x; ++col)
          for(quint64 row = 0; row<_height; ++row)
          {
              exclude(col, row);
          }
        _width = x+1; // new width!
    }

    if(_height < y)
    {
        // this row is not on the map yet --> exclude all added rows
        for(quint64 row = _height; row <= y; ++row)
            for(quint64 col = 0; col <= _width; ++col)
                exclude(col, row);
        _height = y+1;
    }

    if(excluded.contains(x))
    {
        // coordinate is not on the map and the x-ordinate is already excluded
        // -> this execat coordinate was excluded before... remove it from the exclusion
        QList<quint64> l = excluded[x];
        l.removeOne(y);
        excluded[x] = l;
    }

    return;
}

void rune::WorldMap::include(rune::map_coordinate mc)
{
    include(mc.x, mc.y);
    return;
}

QList<rune::map_coordinate> rune::WorldMap::getCoordinateCircle(quint64 x, quint64 y, quint64 radius, bool fill)
{
    #ifdef RUNE_CIRCLE_ALGORITHM_MIDPOINT
    return getMidpointCoordinateCircle(x, y, radius, fill);
    #endif

    #ifdef RUNE_CIRCLE_ALGORITHM_FILTEREDSQUARE
    return getFilteredSquareCoordinateCircle(x, y, radius, fill);
    #endif
}

QList<rune::map_coordinate> rune::WorldMap::getFilteredSquareCoordinateCircle(quint64 x0, quint64 y0, qint64 radius, bool fill)
{
    QList<rune::map_coordinate> points;
    rune::map_coordinate point = {x0, y0};

    for(qint64 x = -radius; x <= radius; ++x)
    {
        for(qint64 y = -radius; y <= radius; ++y)
        {
            // TODO !fill handling
            if(x*x + y*y <= radius*radius)
            {
                rune::map_coordinate ic = {x + point.x, y + point.y};
                points.append(ic);
            }
        }
    }

    return points;
}

QList<rune::map_coordinate> rune::WorldMap::getMidpointCoordinateCircle(quint64 x0, quint64 y0, qint64 radius, bool fill)
{
    QList<rune::map_coordinate> coords;

    quint64 x = radius, y = 0;
    quint64 radiusError = 1-x;

    while(x >= y)
    {
        rune::map_coordinate mc;
        mc.x = x + x0; mc.y = y + y0;
        coords.append(mc);
        mc.x = x + x0; mc.y = y + y0;
        coords.append(mc);
        mc.x = y + x0; mc.y = x + y0;
        coords.append(mc);
        mc.x = -x + x0; mc.y = y + y0;
        coords.append(mc);
        mc.x = -y + x0; mc.y = x + y0;
        coords.append(mc);
        mc.x = -x + x0; mc.y = -y + y0;
        coords.append(mc);
        mc.x = -y + x0; mc.y = -x + y0;
        coords.append(mc);
        mc.x = x + x0; mc.y = -y + y0;
        coords.append(mc);
        mc.x = y + x0; mc.y = -x + y0;
        coords.append(mc);
        y++;
        if (radiusError<0)
        {
          radiusError += 2 * y + 1;
        }
        else {
          x--;
          radiusError+= 2 * (y - x + 1);
        }
    }

    return coords;
}
