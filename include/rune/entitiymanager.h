#ifndef ENTITIYMANAGER_H
#define ENTITIYMANAGER_H

#include "rune/rune_global.h"
#include "yaml-cpp/yaml.h"
#include "rune/entity.h"
#include <QMap>
#include <QString>

namespace rune {
    // global blueprint list
    QMap<QString, Entity*>* g_blueprintRegister = NULL;

    // manages all entities
    class RUNESHARED_EXPORT EntitiyManager
    {
        public:
            EntitiyManager();
            static void init();
            static void cleanUp();
    };
}

#endif // ENTITIYMANAGER_H
