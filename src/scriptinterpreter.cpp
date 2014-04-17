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

    // load script
    QString script = e->getProperty(PROP_SCRIPT);
    if(script.isEmpty())
        return true; // no script

    QFile scrFile(script);
    if(!scrFile.exists())
    {
        rune::setError(RUNE_ERR_NOT_EXISTS);
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
        rune::setError(RUNE_ERR_SCRIPT);
        return false;
    }
    return true;
}

void rune::ScriptInterpreter::call(QString function)
{
    QScriptValue fun = _scriptEngine->globalObject().property(function);
    if(!fun.isFunction())
    {
        rune::setError(RUNE_ERR_SCRIPT);
        return;
    }

    QScriptValue v = fun.call();
    if(v.isError())
    {
        rune::setError(RUNE_ERR_SCRIPT);
    }

    return;
}

QString rune::ScriptInterpreter::scriptPath2FilePath(QString scrPath)
{
    return ""; // TODO
}
