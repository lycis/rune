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


        private:
            QMap<QString, QString> _properties;
    };

}

#endif // ENTITY_H
