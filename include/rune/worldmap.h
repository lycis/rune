#ifndef MAP_H
#define MAP_H

#include "rune/rune_global.h"
#include <QMap>
#include <QList>

namespace rune {

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

        private:
            quint64 _width;
            quint64 _height;

            // x = first key, y = in list -> x: 5 y: 7, 8, 10, ...
            QMap<quint64, QList<quint64>> excluded;
    };
}

#endif // MAP_H
