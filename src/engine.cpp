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
    _init_entities();
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

    _close_entities();

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

bool rune::Engine::loadEntity(QString path)
{
    if(path.isEmpty())
    {
        return false;
    }

    QString loadPath = path;

    if(!loadPath.endsWith(".yml"))
    {
        loadPath += ".yml";
    }else
    {
        path = path.left(path.length()-4);
    }

    if(loadPath.startsWith("/")) {
        loadPath = loadPath.right(loadPath.length()-1);
    }

    // build complete path
    loadPath = _basePath + "entity/" + loadPath;

    QFile yFile(loadPath);
    if(!yFile.exists()  || yFile.size() <= 0)
    {
        qDebug() << "error: entity definition '" << loadPath << "' does not exist" << endl;
        return false;
    }

    // create entity for blueprint base
    Entity* entity = new Entity(this);

    YAML::Node yEntity = YAML::LoadFile(loadPath.toUtf8().constData());

    // inherit from base
    if(yEntity[rune::PROP_BASE.toStdString()])
    {
        QString baseEntity = QString::fromStdString(yEntity[rune::PROP_BASE.toStdString()].as<std::string>());
        if(!g_blueprintRegister->contains(baseEntity)) // load base entity
            if(!loadEntity(baseEntity))
            {
                delete entity;
                return false;
            }
        entity->copyFrom(*getBlueprint(baseEntity));
    }

    // set entity properties from yaml
    for(YAML::iterator it=yEntity.begin(); it != yEntity.end(); ++it){
        entity->setProperty(QString::fromStdString(it->first.as<std::string>()), QString::fromStdString(it->second.as<std::string>()));
    }

    // set entity property
    entity->setProperty(rune::PROP_ENTITY, path);

    if(g_blueprintRegister->contains(path))
    {
        unloadEntity(path);
    }

    // add to blueprint register
    g_blueprintRegister->insert(path, entity);

    return true;
}

bool rune::Engine::unloadEntity(QString path)
{
    if(path.isEmpty())
    {
        return false;
    }

    if(path.endsWith(".yml"))
    {
        path = path.left(path.length()-4);
    }

    if(!g_blueprintRegister->contains(path))
    {
        return false;
    }

    // delete entity
    Entity* e = g_blueprintRegister->value(path);
    g_blueprintRegister->remove(path);

    // TODO remove existing clones?
    delete e;

    return true;
}

QString rune::Engine::cloneEntity(QString path)
{
    if(!g_blueprintRegister->contains(path))
    {
        // blueprint was not loaded yet -> try to load
        if(!loadEntity(path))
            return ""; // blueprint could not be loaded -> error
    }

    // copy entity
    Entity* clone = new Entity(this);
    clone->copyFrom(*(g_blueprintRegister->value(path)));

    /* check if clone register is already available
    if(g_activeEntities == NULL)
    {
        // create if not
        g_activeEntities = new QMap<QString, rune::Entity*>();
    }*/

    // generate entity id
    QUuid uid = QUuid::createUuid();
    clone->setProperty(rune::PROP_UID, uid.toString());

    g_activeEntities->insert(uid.toString(), clone);

    // init interpreter
    ScriptInterpreter* si = new ScriptInterpreter(clone);
    si->bind(clone);

    return clone->getProperty(PROP_UID);
}

bool rune::Engine::modifyEntityProperty(QString uid, QString prop, QString value)
{
    // TODO modification stack
    Entity* e = getClone(uid);
    if(e == NULL)
        return false; // TODO error handling
    e->setProperty(prop, value);
    return true;
}

void rune::Engine::startGameLoop()
{

}

rune::Entity *rune::Engine::getBlueprint(QString path)
{
    if(!g_blueprintRegister->contains(path))
    {
        return NULL;
    }

    return (g_blueprintRegister->value(path));
}

rune::Entity *rune::Engine::getClone(QString uid)
{
    if(!g_activeEntities->contains(uid))
        return NULL;

    return g_activeEntities->value(uid);
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

    WorldMap* map = new WorldMap(this);
    if(!map->loadMap(absPath))
        return NULL;

    map->setName(path);

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

void rune::Engine::_init_entities()
{
    g_blueprintRegister = new QMap<QString, Entity*>();
    g_activeEntities = new QMap<QString, rune::Entity*>();
}

void rune::Engine::_close_entities()
{
    if(g_blueprintRegister == NULL){
        //qWarn("rune entity environment is not initialised");
        return;
    }

    // remove all registered entity blueprints
    // TODO not thread safe! (use mutex or something)
    QList<Entity*> entities = g_blueprintRegister->values();
    for(int i=0; i<entities.size(); ++i){
        delete entities[i];
    }

    delete g_blueprintRegister;
}
