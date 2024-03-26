#ifndef MY_THREADPOOL_H
#define MY_THREADPOOL_H

#include <QObject>
#include <QThreadPool>

class my_threadPool : public QThreadPool
{
    Q_OBJECT
public:
    my_threadPool();
};

#endif // MY_THREADPOOL_H
