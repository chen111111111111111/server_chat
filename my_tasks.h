#ifndef MY_TASKS_H
#define MY_TASKS_H

#endif // MY_TASKS_H
#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlTableModel>
#include <QTcpSocket>
#include <QSqlRecord>
#include <QComboBox>

#include "usr_info.h"


class login_task:public QObject,public QRunnable
{
public:
    login_task(qintptr qptr);
    void data_init(QJsonObject obj,QSqlTableModel *model,QTcpSocket *msocket,QSet<usr_info *> *tree, QComboBox *combobox);

    void run();

    ~login_task();
private:
    QSqlTableModel *model;
    QJsonObject obj;
    QTcpSocket *msocket;
    QSet<usr_info *> *tree;
    QComboBox *combobox;
    qintptr deal_socket;
};


class MtoFriend_task:public QObject,public QRunnable
{
public:
    MtoFriend_task(QJsonObject obj,QSqlTableModel *model,QTcpSocket *msocket,QSet<usr_info *> *tree,QComboBox *combobox);

    void run();

    ~MtoFriend_task();
private:
    QSqlTableModel *model;
    QJsonObject obj;
    QTcpSocket *msocket;
    QSet<usr_info *> *tree;
    QComboBox *combobox;
};
