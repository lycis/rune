#include "rune/entitymanager.h"

// static definitions
QMap<QString, rune::Entity*>* rune::EntityManager::g_blueprintRegister = NULL;
QString rune::EntityManager::_basePath;
rune::str_entity_ll* g_activeEntities = NULL;

rune::EntityManager::EntityManager()
{

}

void rune::EntityManager::init(QString basePath)
{
    // initialise blue print register
    if(g_blueprintRegister == NULL){
        cleanUp();
        //qWarn("rune entity environment is initialised twice");
    }

    g_blueprintRegister = new QMap<QString, Entity*>();

    // configure entity definition base path
    if(!basePath.endsWith("/"))
    {
        basePath += "/";
    }
    _basePath = basePath;
}

void rune::EntityManager::cleanUp()
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

bool rune::EntityManager::loadEntity(QString path)
{
    if(path.isEmpty())
    {
        return false;
    }

    if(!path.endsWith(".yml"))
    {
        path += ".yml";
    }

    QString loadPath = path;
    if(loadPath.startsWith("/")) {
        loadPath = loadPath.right(loadPath.length()-1);
    }

    // build complete path
    loadPath = _basePath + loadPath;

    QFile yFile(loadPath);
    if(!yFile.exists()  || yFile.size() <= 0)
    {
        qDebug() << "error: entity definition '" << loadPath << "' does not exist" << endl;
        return false;
    }

    // build entity from YAML
    Entity* entity = new Entity();
    YAML::Node yEntity = YAML::LoadFile(loadPath.toUtf8().constData());
    for(YAML::iterator it=yEntity.begin(); it != yEntity.end(); ++it){
        entity->setProperty(QString::fromStdString(it->first.as<std::string>()), QString::fromStdString(it->second.as<std::string>()));
    }

    if(g_blueprintRegister->contains(path))
    {
        unloadEntity(path);
    }

    // add to blueprint register
    g_blueprintRegister->insert(path, entity);

    return true;
}

bool rune::EntityManager::unloadEntity(QString path)
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

bool rune::EntityManager::reloadEntity(QString path)
{
    if(path.isEmpty())
    {
        return false;
    }

    if(path.endsWith(".yml"))
    {
        path = path.left(path.length()-4);
    }

    if(g_blueprintRegister->contains(path))
    {
        if(!unloadEntity(path))
            return false;
    }

    return loadEntity(path);
}

rune::Entity *rune::EntityManager::cloneEntity(QString path)
{
    if(!g_blueprintRegister->contains(path))
    {
        // blueprint was not loaded yet -> try to load
        if(!loadEntity(path))
          return NULL; // blueprint could not be loaded -> error
    }

    // copy entity
    Entity* clone = new Entity();
    clone->copyFrom(*(g_blueprintRegister->value(path)));

    // add to list of actives entities
    str_entity_ll* first = g_activeEntities;
    str_entity_ll* cloneRef = new str_entity_ll;
    cloneRef->prev   = NULL;
    cloneRef->entity = clone;
    cloneRef->next   = first;
    g_activeEntities = cloneRef;

    return clone;
}
