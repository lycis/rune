#include "rune/entity.h"

rune::Entity::Entity()
{
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
