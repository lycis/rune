#include "rune/entity.h"

rune::Entity::Entity(Engine *engine)
{
    _interpreter = NULL;
    _engine = engine;
}

/**
 * @brief set a property of the entity
 * @param prop name of the property
 * @param value value to be associated
 */
void rune::Entity::setProperty(QString prop, QString value)
{
    _properties[prop] = value;
}

/**
 * @brief set a proeprty of the entity
 * @param prop name of the property
 * @param value value of the property
 */
void rune::Entity::setProperty(QString prop, qint64 value)
{
    _properties[prop] = QString("%1").arg(value);
}

QString rune::Entity::getProperty(QString prop)
{
    if(!_properties.contains(prop))
        return "";

    return _properties[prop];
}

bool rune::Entity::hasProperty(QString prop)
{
    return _properties.contains(prop);
}

bool rune::Entity::basedOn(QString path)
{
    if(!hasProperty(rune::PROP_BASE))
        return false; // no base entity

    if(getProperty(rune::PROP_ENTITY).compare(path) == 0)
        return true; // this is actually the requested entity

    if(getProperty(rune::PROP_BASE).compare(path) == 0)
        return true; // direct parent is of the requested type

    // search recursively upwards
    return _engine->getBlueprint(getProperty(rune::PROP_BASE))->basedOn(path);
}


rune::ScriptInterpreter *rune::Entity::interpreter() const
{
    return _interpreter;
}

void rune::Entity::setInterpreter(ScriptInterpreter *interpreter)
{
    _interpreter = interpreter;
}

void rune::Entity::call(QString function)
{
    _interpreter->call(function);
}


void rune::Entity::copyFrom(const Entity &other)
{
    for(QMap<QString, QString>::const_iterator it = other._properties.begin(); it != other._properties.end(); ++it)
    {
        _properties[it.key()] = it.value();
    }
}

void rune::Entity::copyFrom(const rune::Entity *other)
{
    copyFrom(*other);
}

rune::Engine *rune::Entity::engine()
{
    return _engine;
}
