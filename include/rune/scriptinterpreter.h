#ifndef SCRIPTINTERPRETER_H
#define SCRIPTINTERPRETER_H

#include "rune/rune_global.h"
#include "rune/entity.h"
#include <QObject>
#include <QScriptEngine>
#include <QFile>
#include <QTextStream>
#include "rune/rune_error.h"

namespace rune {
class Entity;

class RUNESHARED_EXPORT ScriptInterpreter : public QObject
{
    Q_OBJECT
public:
    explicit ScriptInterpreter(QObject *parent = 0);

    /**
     * @brief bind the interpreter to a specific entity
     * @param e
     * @return
     */
    bool bind(Entity* e);

    void call(QString function);

signals:

public slots:

private:
    Entity* _boundEntity;
    QScriptEngine* _scriptEngine;

    QString scriptPath2FilePath(QString scrPath);
};

}

#endif // SCRIPTINTERPRETER_H
