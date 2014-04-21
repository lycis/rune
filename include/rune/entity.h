#ifndef ENTITY_H
#define ENTITY_H

#include "rune/rune_global.h"
#include <QObject>
#include <QMap>
#include <QString>
#include "rune/scriptinterpreter.h"
#include "rune/engine.h"

namespace rune {

    class ScriptInterpreter;
    class Engine;

    static const QString PROP_UID      = "$uid";      //!> system property for the uid of the entity
    static const QString PROP_BASE     = "$base";     //!> system property for the base entity
    static const QString PROP_ENTITY   = "$entity";   //!> system property to identify the entity type
    static const QString PROP_LOCATION = "$location"; //!> system property that stores the map and coordinates of the entity
    static const QString PROP_PASSABLE = "$passable"; //!> system property that tells if the object occupies space
    static const QString PROP_ACTIONS  = "$actions";  //!> system property that specifies actions of the entity
    static const QString PROP_SCRIPT   = "$script";   //!> system property that identifies the associated script file

    // represents one entitiy
    class RUNESHARED_EXPORT Entity : public QObject
    {
    public:
            Entity(Engine* engine);

            /**
             * @brief copies the properties of an other entity
             * @param other
             */
            void copyFrom(const Entity& other);
            void copyFrom(const Entity* other);

            Engine* engine();

    public slots:
            void setProperty(QString prop, QString value);
            void setProperty(QString prop, qint64 value);

            QString getProperty(QString prop);
            bool hasProperty(QString prop);

            /**
             * @brief checks if the given entity is based on the given other entity
             * @param path path of the entity that the current entity should be based on
             * @return
             */
            bool basedOn(QString path);

            ScriptInterpreter *interpreter() const;
            void setInterpreter(ScriptInterpreter *interpreter);

            void call(QString function);

        private:
            Q_OBJECT
            QMap<QString, QString> _properties;
            ScriptInterpreter* _interpreter;
            Engine* _engine;
    };
}

#endif // ENTITY_H
