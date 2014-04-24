#include "rune/worldmap.h"

rune::WorldMap::WorldMap(Engine *parent)
{
    _width  = 0;
    _height = 0;
    _scale  = 0;
    _engine = parent;
    setParent(_engine);
}

rune::WorldMap::WorldMap(QString mapfile, Engine *parent)
{
    _width  = 0;
    _height = 0;
    _scale  = 0;
    _engine = parent;
    setParent(_engine);
    loadMap(mapfile);
}

rune::WorldMap::~WorldMap()
{
}

qint64 rune::WorldMap::width()
{
    return _width;
}

qint64 rune::WorldMap::height()
{
    return _height;
}

void rune::WorldMap::setWidth(qint64 width)
{
    _width = width;
}

void rune::WorldMap::setHeight(qint64 height)
{
    _height = height;
}

bool rune::WorldMap::isPointOnMap(qint64 x, qint64 y)
{
    if(x >= _width || x < 0)
        return false;

    if(y >= _height || y < 0)
        return false;

    // check if coordinate was excluded
    if(excluded.contains(x))
        if(excluded[x].contains(y))
            return false;

    return true;
}

void rune::WorldMap::exclude(qint64 x, qint64 y)
{
    if(!excluded.contains(x))
    {
        // add x coordinate if not already excluding any y coordinates for it
        QList<qint64> l;
        excluded[x] = l;
    }

    QList<qint64> yCoordinates = excluded[x];
    if(yCoordinates.contains(y))
        return; // y-coordinate already excluded

    yCoordinates.append(y);
    excluded[x] = yCoordinates;
    return;
}

void rune::WorldMap::exclude(rune_map_coordinate mc)
{
    exclude(mc.x, mc.y);
    return;
}

void rune::WorldMap::excludeCircle(qint64 x, qint64 y, qint64 radius)
{
    QList<rune_map_coordinate> coords = getCoordinateCircle(x, y, radius);
    for(QList<rune_map_coordinate>::iterator it = coords.begin(); it!=coords.end(); ++it)
    {
        exclude(*it);
    }
    return;
}

void rune::WorldMap::include(qint64 x, qint64 y)
{
    if(isPointOnMap(x, y))
        return; // already part of the map

    if(_width < x)
    {
        // this column is not on the map yet -> exclude all added columns
        for(qint64 col = _width; col <= x; ++col)
          for(qint64 row = 0; row<_height; ++row)
          {
              exclude(col, row);
          }
        _width = x+1; // new width!
    }

    if(_height < y)
    {
        // this row is not on the map yet --> exclude all added rows
        for(qint64 row = _height; row <= y; ++row)
            for(qint64 col = 0; col <= _width; ++col)
                exclude(col, row);
        _height = y+1;
    }

    if(excluded.contains(x))
    {
        // coordinate is not on the map and the x-ordinate is already excluded
        // -> this execat coordinate was excluded before... remove it from the exclusion
        QList<qint64> l = excluded[x];
        l.removeOne(y);
        excluded[x] = l;
    }

    return;
}

void rune::WorldMap::include(rune_map_coordinate mc)
{
    include(mc.x, mc.y);
    return;
}

QList<rune_map_coordinate> rune::WorldMap::getCoordinateCircle(qint64 x, qint64 y, qint64 radius, bool fill)
{
    #ifdef RUNE_CIRCLE_ALGORITHM_MIDPOINT
    return getMidpointCoordinateCircle(x, y, radius, fill);
    #endif

    #ifdef RUNE_CIRCLE_ALGORITHM_FILTEREDSQUARE
    return getFilteredSquareCoordinateCircle(x, y, radius, fill);
#endif
}

qint64 rune::WorldMap::unitToCoordinates(qint64 units, bool diagonal)
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
   for(QMap<qint64, QList<qint64> >::iterator it = excluded.begin(); it != excluded.end(); ++it)
   {
       emitter << YAML::Key << it.key();
       emitter << YAML::Value << YAML::BeginSeq;
       for(QList<qint64>::iterator yIt = it.value().begin(); yIt != it.value().end(); ++yIt)
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
       rune::setError(RUNE_ERR_ACCESS_DENIED);
       return false;
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
        rune::setError(RUNE_ERR_NOT_EXISTS);
        return false;
    }

    YAML::Node yMap = YAML::LoadFile(filename.toUtf8().constData());
    _width  = yMap["width"].as<qint64>();
    _height = yMap["height"].as<qint64>();
    _scale  = yMap["scale"].as<qint64>();

    YAML::Node yExcl = yMap["excluded"];
    if(yExcl.IsMap())
    {
        for(YAML::iterator it=yExcl.begin(); it != yExcl.end(); ++it){
            qint64 x = it->first.as<qint64>();
            YAML::Node yNode = it->first;
            for(YAML::iterator yIt=yNode.begin(); yIt != yNode.end(); ++yIt)
            {
                qint64 y = yIt->first.as<qint64>();
                exclude(x, y);
            }
        }
    }
    return true;
}

bool rune::WorldMap::setEntityPosition(Entity *e, rune_map_coordinate position)
{
    if(!isPointOnMap(position.x, position.y))
        return false;

   rune_map_coordinate currpos = getEntityPosition(e);
    if(currpos.x > -1 && currpos.y > -1)
    {
        // remove from old position
        QList<Entity*> l = _placedEntities[currpos.x][currpos.y];
        l.removeOne(e);
        _placedEntities[currpos.x][currpos.y] = l;
    }


    QList<Entity*> l = _placedEntities[position.x][position.y];

    // check if a non-passable entity occupies this space already
    for(QList<Entity*>::iterator it = l.begin(); it != l.end(); ++it)
    {
        Entity* presentEntity = *it;
        if(presentEntity->getProperty(PROP_PASSABLE).compare("no")==0 ||
           presentEntity->getProperty(PROP_PASSABLE).compare("0")==0)
            return false; // this coordinate can not be entered as there is an unpassable entity blocking it
    }

    if(!l.contains(e))
    {
        l.append(e);
        e->setProperty(PROP_LOCATION, QString("%1:%2:%3").arg(name()).arg(position.x).arg(position.y)); // TODO mapname!
        _placedEntities[position.x][position.y] = l;
    }

    return true;
}

bool rune::WorldMap::setEntityPosition(Entity *e, qint64 x, qint64 y)
{
    rune_map_coordinate mc = {x, y};
    return setEntityPosition(e, mc);
}

rune_map_coordinate rune::WorldMap::getEntityPosition(Entity *e)
{
    rune_map_coordinate my_position;
    QString location = e->getProperty(rune::PROP_LOCATION);
    QStringList parts = location.split(":");
    if(parts.size() != 3)
    {
        my_position.x = -1;
        my_position.y = -1;
        return my_position;
    }

    bool ok;
    qint64 x = parts[1].toLongLong(&ok);
    if(!ok)
    {
        my_position.x = -1;
        my_position.y = -1;
        return my_position;
    }

    qint64 y = parts[2].toLongLong(&ok);
    if(!ok)
    {
        my_position.x = -1;
        my_position.y = -1;
        return my_position;
    }

    my_position.x = x;
    my_position.y = y;
    return my_position;
}

QList<rune::Entity *> rune::WorldMap::getEntitiesInRange(QList<rune_map_coordinate> coords)
{
    QList<Entity*> entities;
    for(QList<rune_map_coordinate>::iterator it = coords.begin(); it != coords.end(); ++it)
    {
        entities += getEntitiesAt(*it);
    }
    return entities;
}

QList<rune::Entity *> rune::WorldMap::getEntitiesAt(rune_map_coordinate pos)
{
    return _placedEntities[pos.x][pos.y];
}

QList<rune::Entity *> rune::WorldMap::getEntitiesAt(qint64 x, qint64 y)
{
    rune_map_coordinate mc = {x, y};
    return getEntitiesAt(mc);
}

QString rune::WorldMap::name() const
{
    return _name;
}

void rune::WorldMap::setName(const QString &name)
{
    _name = name;
}

bool rune::WorldMap::setEntityPosition(QString uid, qint64 x, qint64 y)
{
    Entity* e = _engine->getClone(uid);
    if(e == NULL)
    {
        rune::setError(RUNE_ERR_NOT_EXISTS,
                       QString("entity [%1] does not exist").arg(uid));
        return false;
    }
    return setEntityPosition(e, x, y);
}


qint64 rune::WorldMap::scale() const
{
    return _scale;
}

void rune::WorldMap::setScale(const qint64 &scale)
{
    _scale = scale;
}


QList<rune_map_coordinate> rune::WorldMap::getFilteredSquareCoordinateCircle(qint64 x0, qint64 y0, qint64 radius, bool fill)
{
    QList<rune_map_coordinate> points;
    rune_map_coordinate point = {x0, y0};

    for(qint64 x = -radius; x <= radius; ++x)
    {
        for(qint64 y = -radius; y <= radius; ++y)
        {
            // TODO !fill handling
            if(x*x + y*y <= radius*radius)
            {
                rune_map_coordinate ic = {x + point.x, y + point.y};
                points.append(ic);
            }
        }
    }

    return points;
}

QList<rune_map_coordinate> rune::WorldMap::getMidpointCoordinateCircle(qint64 x0, qint64 y0, qint64 radius, bool fill)
{
    QList<rune_map_coordinate> coords;

    qint64 x = radius, y = 0;
    qint64 radiusError = 1-x;

    while(x >= y)
    {
        rune_map_coordinate mc;
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
