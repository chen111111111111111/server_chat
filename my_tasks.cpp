#include "my_tasks.h"

login_task::login_task(QJsonObject obj,QSqlTableModel *model,QTcpSocket *msocket):obj(obj),model(model),msocket(msocket)
{
    setAutoDelete(true);
}

void login_task::run()
{
    int a = ui->comboBox->currentIndex();   //记录当前界面，用以复原
    ui->comboBox->setCurrentIndex(0);
    QJsonObject obj_message = obj.value("login").toObject();
    QString usr = obj_message.value("usr").toString();
    QString pwd = obj_message.value("pwd").toString();
    qDebug()<<usr<<" "<<pwd;
    QString table = model->tableName();
    model->setTable("user");
    model->setFilter(QString("usr = '%1' and pwd = '%2'").arg(usr).arg(pwd));
    model->select();
    if(model->rowCount()>0)
    {
        QJsonObject obj;
        obj.insert("result","ok");
        obj.insert("name",model->record(0).value("name").toString());
        QJsonDocument doc(obj);
        msocket->write(doc.toJson());
        usr_info *usr1 = new usr_info;
        usr1->Usr = usr;
        usr1->msocket = msocket;
        tree.insert(usr1);
    }
    else
    {
        QJsonObject obj;
        obj.insert("result","failed");
        QJsonDocument doc(obj);
        msocket->write(doc.toJson());
    }
    model->setTable(table);
    model->setFilter("");
    model->select();
    ui->comboBox->setCurrentIndex(a);
}

login_task::~login_task()
{
    //qDebug()<<"任务自动析构";
}
