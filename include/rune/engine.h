#ifndef RUNE_H
#define RUNE_H

#include "rune_global.h"
#include "entitymanager.h"
#include "worldmap.h"
#include <QString>
#include "rune_error.h"

namespace rune {


    /**
     * @brief This class manages all global settings and actions.
     * Everything that is not specific to any module will be controlled here.
     */
    class RUNESHARED_EXPORT Engine : public QObject
    {
        Q_OBJECT

        public:
            Engine();
            ~Engine();

            /**
             * @brief initialises the engine
             */
            void init();

            /**
             * @brief change the base path
             * worthless after calling init
             * @param basePath
             */
            void setBasePath(QString basePath);

            /**
             * @brief gives the base path of the engine
             */
            QString basePath();

            /**
             * @brief clean up and close all engine processing
             */
            void close();

            /**** ENTITY MANAGEMENT ****/

            /**
             * @brief load the definition of an entity into the blueprint register
             * When loading an already loaded entity it will be reloaded.
             * @param path path to the entity definition within the base directory (/ is the root for entities)
             * @return <code>true</code> if load was successful
             */
            bool loadEntity(QString path);

            /**
             * @brief unload the definition of an entity from the blueprint register
             * @param path
             * @return
             */
            bool unloadEntity(QString path);

            /**
             * @brief instantiates a new clone of the given blueprint
             * the blueprint will be loaded if it was not already
             * @param path blueprint path
             * @return pointer to the created entity
             */
            Entity* cloneEntity(QString path);

            /**
             * @brief gives a reference to a blueprint entity
             * this allows modifying a blueprint that was already loaded at runtime
             * without modification of the actual file. all entities that are cloned
             * from this blueprint after it was modified will inherit the changed
             * values instead those defined in the file.
             *
             * @param path
             */
            Entity* getBlueprint(QString path);

    public slots:
            WorldMap* loadMap(QString path);
            void      unloadMap(QString path);
            WorldMap* getMap(QString path);

        private:
            QString _basePath;
            QMap<QString, WorldMap*> _loadedMaps;
            bool _open;
            QMap<QString, rune::Entity*>* g_blueprintRegister;
            QMap<QString, rune::Entity*>* g_activeEntities;
    };
}

#endif // RUNE_H
