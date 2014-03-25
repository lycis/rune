#include "rune/engine.h"

QString rune::Engine::_basePath;


rune::Engine::Engine()
{
    _basePath = "./";
}

void rune::Engine::init()
{
    EntityManager::init(QString("%1entity/").arg(_basePath));
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
    EntityManager::cleanUp();
}
