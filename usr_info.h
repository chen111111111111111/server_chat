#ifndef USR_INFO_H
#define USR_INFO_H

#include <QTcpSocket>
class usr_info
{
public:
    usr_info();
    QString Usr;
    QTcpSocket *msocket;
};

#endif // USR_INFO_H
