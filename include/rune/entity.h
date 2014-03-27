#ifndef ENTITY_H
#define ENTITY_H

#include "rune/rune_global.h"
#include <QMap>
#include <QString>
#include "rune/entitymanager.h"

namespace rune {

    static const QString PROP_UID    = "$uid";    //!> system property for the uid of the entity
    static const QString PROP_BASE   = "$base";   //!> system property for the base entity
    static const QString PROP_ENTITY = "$entity"; //!> system property to identify the entity type

    // represents one entitiy
    class RUNESHARED_EXPORT Entity
    {
        public:
            Entity();

            void setProperty(QString prop, QString value);
            void setProperty(QString prop, qint64 value);

            QString getProperty(QString prop);
            bool hasProperty(QString prop);

            /**
             * @brief checks if the given entity is based on the given other entity
             * @param path path of the entity that the current entity should be based on
             * @return
             */
            bool basedOn(QString path);


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
