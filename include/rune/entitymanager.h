#ifndef ENTITIYMANAGER_H
#define ENTITIYMANAGER_H

#include "rune/rune_global.h"
#include "yaml-cpp/yaml.h"
#include "rune/entity.h"
#include <QMap>
#include <QString>
#include <QFile>

namespace rune {
    // manages all entities
    class RUNESHARED_EXPORT EntityManager
    {
        public:
            EntityManager();

            /**
             * @brief initialise the entity environment
             */
            static void init(QString basePath);

            /**
             * @brief destruct entity environment and clean up
             */
            static void cleanUp();

            /**
             * @brief load the definition of an entity into the blueprint register
             * @param path path to the entity definition within the base directory (/ is the root for entities)
             * @return <code>true</code> if load was successful
             */
            static bool loadEntity(QString path);

            /**
             * @brief unload the definition of an entity from the blueprint register
             * @param path
             * @return
             */
            static bool unloadEntity(QString path);

            /**
             * @brief unloads and reloads the specified entity.
             * it will be loaded only if it was not loaded beforehand
             * @param path
             * @return
             */
            static bool reloadEntity(QString path);

        private:
            static QString _basePath;
            static QMap<QString, rune::Entity*>* g_blueprintRegister; // global blueprint list
    };
}

#endif // ENTITIYMANAGER_H
