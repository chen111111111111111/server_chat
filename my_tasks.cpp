#include "my_tasks.h"

login_task::login_task(qintptr qptr)
{
    setAutoDelete(true);
    deal_socket = qptr;
}

void login_task::data_init(QJsonObject obj,QSqlTableModel *model,QTcpSocket *msocket,QSet<usr_info *> *tree, QComboBox *combobox)
{
    this->obj = obj;
    this->tree = tree;
    this->msocket = msocket;
    this->model = model;
    this->combobox = combobox;

    //使能socket
    msocket->setSocketDescriptor(deal_socket);
}

void login_task::run()
{

}

login_task::~login_task()
{
    qDebug()<<"任务自动析构";
}

MtoFriend_task::MtoFriend_task(QJsonObject obj, QSqlTableModel *model, QTcpSocket *msocket, QSet<usr_info *> *tree, QComboBox *combobox):model(model),obj(obj),msocket(msocket),tree(tree),combobox(combobox)
{
    setAutoDelete(true);
}

void MtoFriend_task::run()
{

}

MtoFriend_task::~MtoFriend_task()
{
    qDebug()<<"任务自动析构";
}


