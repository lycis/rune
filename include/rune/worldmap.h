#ifndef MAP_H
#define MAP_H

#include "rune/rune_global.h"

namespace rune {
    class RUNESHARED_EXPORT WorldMap
    {
        public:
            WorldMap();

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



        private:
            quint64 _width;
            quint64 _height;
    };
}

#endif // MAP_H
