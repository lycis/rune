#ifndef GAMELOOPTHREAD_H
#define GAMELOOPTHREAD_H

#include <QThread>
#include "rune/engine.h"

namespace rune {
class Engine;

class GameLoopThread : public QThread
{
    Q_OBJECT
public:
    explicit GameLoopThread(Engine *parent = 0);

signals:
    void gameLoopFinished();

public slots:
    void invalidateEngine();

private:
    void run();

    Engine* _engine;
    bool _engineValid;

};

};

#endif // GAMELOOPTHREAD_H
