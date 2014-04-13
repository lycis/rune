#include "rune/engine.h"


rune::Engine::Engine()
{
    _basePath = "./";
}

rune::Engine::~Engine()
{
    close();
}

void rune::Engine::init()
{
    EntityManager::init(QString("%1entity/").arg(_basePath));
    _open = true;
}

void rune::Engine::setBasePath(QString basePath)
{
    if(!basePath.endsWith("/"))
    {
        _basePath += "/";
    }

    _basePath = basePath;
}

QString rune::Engine::basePath()
{
    return _basePath;
}

void rune::Engine::close()
{
    if(!_open)
        return;

    EntityManager::cleanUp();

    if(_loadedMaps.size() > 0)
    {
        // unload all loaded maps
        for(QMap<QString, WorldMap*>::iterator it = _loadedMaps.begin();
            it != _loadedMaps.end();
            ++it)
        {
            qDebug() << "free map " << it.key() << " at " << it.value();
            delete it.value();
        }
    }
    _open = false;
}

rune::WorldMap *rune::Engine::loadMap(QString path)
{
    QString absPath = path;

    if(!path.startsWith("/"))
        path = "/" + path;

    if(_loadedMaps.contains(path))
    {
        if(_loadedMaps[path] != NULL)
        {
            rune::setError(RUNE_ERR_MAP_LOADED_BEFORE);
            return NULL;
        }
    }

    absPath = path.right(path.length() - 1);
    absPath = _basePath + "map/" + absPath + ".yml";

    qDebug() << "engine::load_map <- " << absPath;

    WorldMap* map = new WorldMap();
    if(!map->loadMap(absPath))
        return NULL;

    _loadedMaps[path] = map;
    return map;
}

void rune::Engine::unloadMap(QString path)
{
    if(!path.startsWith("/"))
        path = "/" + path;

    if(!_loadedMaps.contains(path))
        return;

    WorldMap* map = _loadedMaps[path];
    if(map == NULL)
        return;

    delete map;
    _loadedMaps[path] = NULL;
    return;
}

rune::WorldMap *rune::Engine::getMap(QString path)
{
    if(!path.startsWith("/"))
        path = "/" + path;

    if(!_loadedMaps.contains(path))
        return NULL;

    return _loadedMaps[path];
}
