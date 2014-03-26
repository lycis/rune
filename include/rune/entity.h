#ifndef ENTITY_H
#define ENTITY_H

#include "rune/rune_global.h"
#include <QMap>
#include <QString>

namespace rune {

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
