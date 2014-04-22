#include "rune/scriptinterpreter.h"

rune::ScriptInterpreter::ScriptInterpreter(QObject *parent) :
    QObject(parent)
{
    _boundEntity = NULL;
    _scriptEngine = new QScriptEngine(this);
}

bool rune::ScriptInterpreter::bind(Entity *e)
{
    _boundEntity = e;
    e->setInterpreter(this);

    // bind global reference variables

    // reference to this entity
    QScriptValue thisRef = _scriptEngine->newQObject(e);
    _scriptEngine->globalObject().setProperty("me", thisRef);

    // reference to engine
    QScriptValue engineRef = _scriptEngine->newQObject(e->engine());
    _scriptEngine->globalObject().setProperty("engine", engineRef);

    // load script
    QString script = e->getProperty(PROP_SCRIPT);
    if(script.isEmpty())
        return true; // no script

    QFile scrFile(script);
    if(!scrFile.exists())
    {
        rune::setError(RUNE_ERR_NOT_EXISTS,
                       QString("script [%1] referenced in entity ([%2] %3) not found")
                       .arg(script).arg(e->getProperty(PROP_ENTITY)).arg(e->getProperty(PROP_UID)));
        return false;
    }

    if(!scrFile.open(QIODevice::ReadOnly))
    {
        rune::setError(RUNE_ERR_ACCESS_DENIED);
        return false;
    }

    QTextStream scrStream(&scrFile);
    QString scrStr = scrStream.readAll();
    QScriptValue v = _scriptEngine->evaluate(scrStr);
    if(v.isError())
    {
        rune::setError(RUNE_ERR_SCRIPT,
                       QString("binding script (%1) to entity ([%2] %3) failed: %4")
                       .arg(script).arg(e->getProperty(PROP_ENTITY)).arg(e->getProperty(PROP_UID))
                       .arg(v.toString()));
        return false;
    }
    return true;
}

void rune::ScriptInterpreter::call(QString function)
{
    QScriptValue fun = _scriptEngine->globalObject().property(function);
    if(!fun.isFunction())
    {
        rune::setError(RUNE_ERR_SCRIPT,
                       QString("action call for entity ([%1] %2) failed: unknown function '%3'")
                       .arg(_boundEntity->getProperty(PROP_ENTITY))
                       .arg(_boundEntity->getProperty(PROP_UID))
                       .arg(function));
        return;
    }

    if(_boundEntity != NULL)
    {
        // provide reference to current map if bound to entity
        QString map = _boundEntity->getProperty(PROP_LOCATION);
        map = map.split(":")[0];
        if(!map.isEmpty())
        {
            WorldMap* m = _boundEntity->engine()->getMap(map);
            QScriptValue mapRef = _scriptEngine->newQObject(m);
            _scriptEngine->globalObject().setProperty("map", mapRef);
        }else
        {
            QScriptValue mapRef = _scriptEngine->nullValue();
            _scriptEngine->globalObject().setProperty("map", mapRef);
        }
    }

    QScriptValue v = fun.call();
    if(v.isError())
    {
        rune::setError(RUNE_ERR_SCRIPT,
                       QString("script error in entity ([%1] %2), function '%3': %4")
                       .arg(_boundEntity->getProperty(PROP_ENTITY))
                       .arg(_boundEntity->getProperty(PROP_UID))
                       .arg(function)
                       .arg(v.toString()));
    }

    return;
}

QString rune::ScriptInterpreter::scriptPath2FilePath(QString scrPath)
{
    return ""; // TODO
}
