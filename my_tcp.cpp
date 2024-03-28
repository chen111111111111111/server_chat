#include "my_tcp.h"
#include <QThreadPool>
#include "my_tasks.h"
my_tcp::my_tcp()
{
}

void my_tcp::incomingConnection(qintptr qptr)
{
    QThreadPool::globalInstance()->start(new login_task(qptr));
}
