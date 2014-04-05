#include "rune/worldmap.h"

rune::WorldMap::WorldMap()
{
    _width  = 0;
    _height = 0;
    _scale  = 0;
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

quint64 rune::WorldMap::unitToCoordinates(quint64 units, bool diagonal)
{
    return units*_scale;
}

bool rune::WorldMap::saveMap(QString filename)
{
    // create yaml
   YAML::Emitter emitter;
   emitter << YAML::BeginMap;

   emitter << YAML::Key << "width";
   emitter << YAML::Value << _width;

   emitter << YAML::Key << "height";
   emitter << YAML::Value << _height;

   emitter << YAML::Key << "scale";
   emitter << YAML::Value << _scale;

   emitter << YAML::Key << "excluded";
   emitter << YAML::Value << YAML::BeginMap;
   for(QMap<quint64, QList<quint64> >::iterator it = excluded.begin(); it != excluded.end(); ++it)
   {
       emitter << YAML::Key << it.key();
       emitter << YAML::Value << YAML::BeginSeq;
       for(QList<quint64>::iterator yIt = it.value().begin(); yIt != it.value().end(); ++yIt)
       {
           emitter << *yIt;
       }
       emitter << YAML::EndSeq;
   }
   emitter << YAML::EndSeq;

   emitter << YAML::EndMap;

   // write to file
   QFile yamlFile(filename);
   if(!yamlFile.open(QIODevice::Truncate|QIODevice::ReadWrite))
   {
       return false; // TODO erro rhandling
   }

   QTextStream str(&yamlFile);
   str << emitter.c_str();
   return true;
}

bool rune::WorldMap::loadMap(QString filename)
{
    QFile ymlFile(filename);
    if(!ymlFile.exists())
    {
        return false; // TODO error handling
    }

    YAML::Node yMap = YAML::LoadFile(filename.toUtf8().constData());
    _width  = yMap["width"].as<quint64>();
    _height = yMap["height"].as<quint64>();
    _scale  = yMap["scale"].as<quint64>();

    YAML::Node yExcl = yMap["excluded"];
    if(!yExcl.IsMap())
    {
        return false; // TODO error handling
    }

    for(YAML::iterator it=yExcl.begin(); it != yExcl.end(); ++it){
        quint64 x = it->first.as<quint64>();
        YAML::Node yNode = it->first;
        for(YAML::iterator yIt=yNode.begin(); yIt != yNode.end(); ++yIt)
        {
           quint64 y = yIt->first.as<quint64>();
           exclude(x, y);
        }
    }
    return true;
}

quint64 rune::WorldMap::scale() const
{
    return _scale;
}

void rune::WorldMap::setScale(const quint64 &scale)
{
    _scale = scale;
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
