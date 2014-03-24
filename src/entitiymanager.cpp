#include "rune/entitiymanager.h"

rune::EntitiyManager::EntitiyManager()
{
    YAML::Node test = YAML::LoadFile("test.yml");
}

/**
 * @brief initialise the entity environment
 */
void rune::EntitiyManager::init()
{
    if(g_blueprintRegister == NULL){
        cleanUp();
        //qWarn("rune entity environment is initialised twice");
    }

    g_blueprintRegister = new QMap<QString, Entity*>();
}

/**
 * @brief destruct entity environment and clean up
 */
void rune::EntitiyManager::cleanUp()
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
