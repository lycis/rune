#ifndef RUNE_H
#define RUNE_H

#include "rune_global.h"
#include "worldmap.h"
#include <QString>
#include "rune_error.h"
#include <QUuid>
#include "entity.h"
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include "rune/gameloopthread.h"

struct rune_action_queue_item {
    QString uid;
    QString action;
    uint timestamp;
};

namespace rune {
   class WorldMap;
   class Entity;
   class GameLoopThread;


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

            /**
             * @brief retrungs a reference to the entity by uid
             * @param uid
             */
            Entity* getClone(QString uid);

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

            /**
             * @brief returns a queue of actions that is ready for execution
             */
            QQueue<rune_action_queue_item> getReadyActions();

    signals:
            void gameStateChanged(); //!> emitted whenever the game state changed

    public slots:

            /**** MAP MANAGEMENT ****/
            WorldMap* loadMap(QString path);
            void      unloadMap(QString path);
            WorldMap* getMap(QString path);

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
            QString cloneEntity(QString path);

            /**
             * @brief changes the value of a property of an entity without a direct pointer to it
             * exists mainly for scripting support
             * @param uid id of the entity
             * @param prop name of property
             * @param value new value of property
             * @return
             */
            bool modifyEntityProperty(QString uid, QString prop, QString value);

            /**** Game Loop ****/
            /**
             * @brief starts the overall game loop
             */
            void startGameLoop();

            /**
             * @brief stops an already active game loop
             */
            void stopGameLoop();

            /**** Entity Interaction ****/
            /**
             * @brief enqueue an action to be executed
             * @param uid id if the entity
             * @param action action to call within the entity
             * @param offset offset in seconds until this action should be executed from now on
             */
            void callAction(QString uid, QString action, uint offset = 0);

        private slots:
            void glFinished();

        private:
            QString _basePath;
            QMap<QString, WorldMap*> _loadedMaps;
            bool _open;
            QMap<QString, rune::Entity*>* g_blueprintRegister;
            QMap<QString, rune::Entity*>* g_activeEntities;

            QQueue<rune_action_queue_item> _actionQueue;
            QMutex _actionQueueMutex;

            GameLoopThread* _glThread;

            void _init_entities();
            void _close_entities();
    };
}

#endif // RUNE_H
