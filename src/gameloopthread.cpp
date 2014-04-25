#include "rune/gameloopthread.h"

rune::GameLoopThread::GameLoopThread(Engine *parent) :
    QThread(NULL)
{
    _engine = parent;
    _engineValid = true;
}

void rune::GameLoopThread::invalidateEngine()
{
    _engineValid = false;
}

void rune::GameLoopThread::run()
{
    if(_engine == NULL)
        return;

    while(_engineValid)
    {
        QQueue<rune_action_queue_item> queue = _engine->getReadyActions();
        bool stateChange = false;
        while(!queue.isEmpty() && _engineValid)
        {
            rune_action_queue_item actItem = queue.dequeue();
            Entity* e = _engine->getClone(actItem.uid);
            // TODO create script interpreter and bind it here to reduce used memory by entity
            e->call(actItem.action);
            stateChange = true;
        }
        if(stateChange) emit(gameLoopFinished()); // send signal to enginge
    }

}
