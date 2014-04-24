#ifndef MAP_H
#define MAP_H

#include "rune/rune_global.h"
#include <QMap>
#include <QList>
#include "yaml-cpp/yaml.h"
#include <QFile>
#include "rune/entity.h"
#include <QStringList>
#include "rune_error.h"
#include "rune/engine.h"

struct rune_map_coordinate //!> coordinate on a map
{
    qint64 x;
    qint64 y;
};

Q_DECLARE_METATYPE(rune_map_coordinate)

namespace rune {
    class Entity;
    class Engine;

    class RUNESHARED_EXPORT WorldMap : public QObject
    {
        public:
            WorldMap(Engine* parent = 0);
            WorldMap(QString mapfile, Engine* parent = 0);
            ~WorldMap();

            /**
             * @brief gives the total width of the map
             */
            qint64 width();

            /**
             * @brief gives the total height of the map
             */
            qint64 height();

            /**
             * @brief change width of a loaded map
             * @param width
             */
            void setWidth(qint64 width);

            /**
             * @brief change height of a loaded map
             * @param height
             */
            void setHeight(qint64 height);

            /**
             * @brief exclude the given coordinate from the map
             * @param mc
             */
            void exclude(rune_map_coordinate mc);

            void excludeCircle(qint64 x, qint64 y, qint64 radius);

            /**
             * @brief include the given coordinate
             * @param mc coordinate
             */
            void include(rune_map_coordinate mc);


            QList<rune_map_coordinate> getCoordinateCircle(qint64 x, qint64 y, qint64 radius, bool fill = true);

            qint64 scale() const;
            void setScale(const qint64 &scale);

            /**
             * @brief translates units (e.g. meters, foot, miles) into a coordinate distance
             * @param units
             */
            qint64 unitToCoordinates(qint64 units, bool diagonal);

            /**
             * @brief save the map to a file
             * @param filename
             */
            bool saveMap(QString filename);

            /**
             * @brief load a map from the given file
             * @param filename
             * @return
             */
            bool loadMap(QString filename);

            bool setEntityPosition(Entity* e, rune_map_coordinate position);
            bool setEntityPosition(Entity* e, qint64 x, qint64 y);
            rune_map_coordinate getEntityPosition(Entity* e);

            QList<Entity*> getEntitiesInRange(QList<rune_map_coordinate> coords);
            QList<Entity*> getEntitiesAt(rune_map_coordinate pos);
            QList<Entity*> getEntitiesAt(qint64 x, qint64 y);

            QString name() const;
            void setName(const QString &name);

    public slots:
            /**
             * @brief change the position of an entity identified by its id
             * @param uid id of the entity
             * @param x
             * @param y
             * @return
             */
            bool setEntityPosition(QString uid, qint64 x, qint64 y);

            /**
             * @brief checks if the given coordinate is on the map
             * @param x
             * @param y
             */
            bool isPointOnMap(qint64 x, qint64 y);

            /**
             * @brief include the given coordinate
             * @param x
             * @param y
             */
            void include(qint64 x, qint64 y);

            /**
             * @brief exclude certain coordinates from the map
             * @param x
             * @param y
             */
            void exclude(qint64 x, qint64 y);

            /**
             * @brief provides the position of the entity based on the entity uid
             * @param uid
             * @return
             */
            rune_map_coordinate getEntityPosition(QString uid);

    private:
            Q_OBJECT

            qint64 _width;
            qint64 _height;
            qint64 _scale; // 1 coordnate takes _scale square units (e.g. _scale = 5 (foot) -> 1 coordnate takes 5 square foot)
            QString _name;
            Engine* _engine;

            // x = first key, y = in list -> x: 5 y: 7, 8, 10, ...
            QMap<qint64, QList<qint64> > excluded;

            QMap<qint64, QMap<qint64, QList<Entity*> > > _placedEntities;

            /**
             * @brief returns a list of coordinates on the circle
             * if fill is true all coordinates within the circle will be returned
             * @param x x-ordinate of center
             * @param y y-ordinate of center
             * @param radius radius in coordinate
             * @param fill
             */
            QList<rune_map_coordinate> getMidpointCoordinateCircle(qint64 x, qint64 y, qint64 radius, bool fill = true);
            QList<rune_map_coordinate> getFilteredSquareCoordinateCircle(qint64 x0, qint64 y0, qint64 radius, bool fill = true);
    };
}

#endif // MAP_H
