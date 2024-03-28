#ifndef MY_TCP_H
#define MY_TCP_H

#include <QTcpServer>
class my_tcp :public QTcpServer
{
public:
    explicit my_tcp();
    void incomingConnection(qintptr qptr);

signals:

};

#endif // MY_TCP_H
