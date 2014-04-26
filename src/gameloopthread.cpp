#include "rune/gameloopthread.h"

rune::GameLoopThread::GameLoopThread(Engine *parent) :
    QThread(NULL)
{
    _engine = parent;
    _engineValid = true;
}

void rune::GameLoopThread::invalidateEngine()
{
    qDebug() << "invalidateEngine";
    _engineValid = false;
}

void rune::GameLoopThread::run()
{
    if(_engine == NULL)
        return;

    qDebug() << "gameloop started running";

    while(_engineValid)
    {
        QQueue<rune_action_queue_item> queue = _engine->getReadyActions();
        //qDebug() << "ready actions: " << queue.size();
        bool stateChange = false;
        while(!queue.isEmpty() && _engineValid)
        {
            rune_action_queue_item actItem = queue.dequeue();
            Entity* e = _engine->getClone(actItem.uid);
            // TODO create script interpreter and bind it here to reduce used memory by entity
            e->call(actItem.action);
            stateChange = true;
        }
        if(stateChange) _engine->fireGameStateChanged(); // send signal to engine
    }
    qDebug() << "gameloop end";
}
