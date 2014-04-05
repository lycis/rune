#ifndef MAP_H
#define MAP_H

#include "rune/rune_global.h"
#include <QMap>
#include <QList>
#include "yaml-cpp/yaml.h"
#include <QFile>

namespace rune {
    struct map_coordinate //!> coordinate on a map
    {
        quint64 x;
        quint64 y;
    };

    class RUNESHARED_EXPORT WorldMap
    {
        public:
            WorldMap();
            ~WorldMap();

            /**
             * @brief gives the total width of the map
             */
            quint64 width();

            /**
             * @brief gives the total height of the map
             */
            quint64 height();

            /**
             * @brief change width of a loaded map
             * @param width
             */
            void setWidth(quint64 width);

            /**
             * @brief change height of a loaded map
             * @param height
             */
            void setHeight(quint64 height);

            /**
             * @brief checks if the given coordinate is on the map
             * @param x
             * @param y
             */
            bool isPointOnMap(quint64 x, quint64 y);

            /**
             * @brief exclude certain coordinates from the map
             * @param x
             * @param y
             */
            void exclude(quint64 x, quint64 y);
            void exclude(map_coordinate mc);

            void excludeCircle(quint64 x, qint64 y, quint64 radius);

            void include(quint64 x, quint64 y);
            void include(rune::map_coordinate mc);


            QList<map_coordinate> getCoordinateCircle(quint64 x, quint64 y, quint64 radius, bool fill = true);

            quint64 scale() const;
            void setScale(const quint64 &scale);

            /**
             * @brief translates units (e.g. meters, foot, miles) into a coordinate distance
             * @param units
             */
            quint64 unitToCoordinates(quint64 units, bool diagonal);

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
    private:
            quint64 _width;
            quint64 _height;
            quint64 _scale; // 1 coordnate takes _scale square units (e.g. _scale = 5 (foot) -> 1 coordnate takes 5 square foot)

            // x = first key, y = in list -> x: 5 y: 7, 8, 10, ...
            QMap<quint64, QList<quint64> > excluded;

            /**
             * @brief returns a list of coordinates on the circle
             * if fill is true all coordinates within the circle will be returned
             * @param x x-ordinate of center
             * @param y y-ordinate of center
             * @param radius radius in coordinate
             * @param fill
             */
            QList<map_coordinate> getMidpointCoordinateCircle(quint64 x, quint64 y, qint64 radius, bool fill = true);
            QList<map_coordinate> getFilteredSquareCoordinateCircle(quint64 x0, quint64 y0, qint64 radius, bool fill = true);
    };
}

#endif // MAP_H
