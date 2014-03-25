#ifndef RUNE_H
#define RUNE_H

#include "rune_global.h"
#include "entitymanager.h"
#include <QString>

namespace rune {
    /**
     * @brief This class manages all global settings and actions.
     * Everything that is not specific to any module will be controlled here.
     */
    class RUNESHARED_EXPORT Engine
    {
        public:
            Engine();

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
        private:
            static QString _basePath;
    };
}

#endif // RUNE_H
