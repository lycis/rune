#ifndef ENTITY_H
#define ENTITY_H

#include "rune/rune_global.h"
#include <QMap>
#include <QString>

namespace rune {

    static const QString PROP_UID = "$uid"; //!> system property for the uid of the entity
    static const QString PROP_BASE = "$base"; //!> system property for the base entity

    // represents one entitiy
    class RUNESHARED_EXPORT Entity
    {
        public:
            Entity();

            void setProperty(QString prop, QString value);
            void setProperty(QString prop, qint64 value);

            QString getProperty(QString prop);


            /**
             * @brief copies the properties of an other entity
             * @param other
             */
            void copyFrom(const Entity other);

        private:
            QMap<QString, QString> _properties;
    };

}

#endif // ENTITY_H
