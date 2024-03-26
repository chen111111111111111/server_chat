#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlRecord>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //窗口初始化
    ui->setupUi(this);
    resize(800,600);
    setWindowTitle("聊天服务器");
    user_w= new user_win(this) ;
    widget = user_w;
    widget->setGeometry(0,20,800,200);
    connect(user_w,&user_win::add_send,this,&MainWindow::add_usr);
    connect(user_w,&user_win::select_send,this,&MainWindow::select_usr);
    connect(user_w,&user_win::update_send,this,&MainWindow::update_usr);
    connect(user_w,&user_win::delete_send,this,&MainWindow::delete_usr);
    relation_win = new relations(this);
    connect(relation_win,&relations::relation_select,this,&MainWindow::relation_select);
    relation_win->hide();
    group_w = new group_win(this);
    group_w ->hide();
    d_m_w = new disDeal_message_win(this);
    d_m_w->hide();
    info_win = new infomation(this);
    connect(info_win,&infomation::info_select,this,&MainWindow::info_select);
    info_win->hide();
    //数据库初始化
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat.db");
    db.open();
    if(!db.isOpen())qDebug()<<"打开数据库失败";
    else qDebug()<<"打开数据库成功";

    //创建对应的表
    QString sql = QString("create table user(name text,usr text,pwd text);");
    db.exec(sql);
    sql = QString("create table relations(name1 text,usr1 text,name2 text,usr2 text,type text);");
    db.exec(sql);
    sql = QString("create table messages(send_name text,send_usr text,recv_name text,recv_usr text,text text,type text,time text,old_send_name text,old_send_usr text);");
    db.exec(sql);
    sql = QString("create table infomation(send_name text,send_usr text,recv_name text,recv_usr text,text text,type text,time text,old_send_name text,old_send_usr text);");
    db.exec(sql);
    sql = QString("create table groups(group_name text,group_id text,members text);");
    db.exec(sql);
    //数据库显示初始化
    model = new QSqlTableModel(this,db);
    ui->tableView->setModel(model);
    model->setTable("user");
    model->select();

    //开始建立系统聊天室
    model->setTable("groups");
    model->setFilter(QString("group_id='00000001'"));
    model->select();
    if(model->rowCount()==0)
    {
        QSqlRecord record = model->record(0);
        record.setValue("group_name","系统聊天室");
        record.setValue("group_id","00000001");
        record.setValue("members",0);
        model->insertRecord(0,record);
    }
    model->setTable("user");
    model->select();


    //网络服务器初始化
    mserver.listen(QHostAddress::Any,60000);
    connect(&mserver,&QTcpServer::newConnection,this,&MainWindow::tcp_server);
}

MainWindow::~MainWindow()
{
    for (QSet<usr_info*>::iterator item = tree.begin(); item!=tree.end();item++) {
        (*item)->msocket->close();
        delete (*item);
    }

    delete ui;
    delete model;
    delete widget;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    widget->hide();
    if(arg1 == "用户管理")
    {
        widget = user_w;
        widget ->setGeometry(0,20,800,200);
        widget->show();
        model->setTable("user");
        model->select();
    }
    else if(arg1 == "未处理消息管理")
    {
        widget = d_m_w;
        widget ->setGeometry(0,20,800,200);
        widget->show();
        //connect(dynamic_cast<user_win*>(widget),&user_win::add_send,this,&MainWindow::add_usr);
        model->setTable("messages");
        model->select();
    }
    else if(arg1 == "群聊管理")
    {
        widget = group_w;
        widget ->setGeometry(0,20,800,200);
        widget->show();
        model->setTable("groups");
        model->select();
    }
    else if(arg1 == "消息历史记录")
    {
        widget = info_win;
        widget ->setGeometry(0,20,800,200);
        widget->show();
        model->setTable("infomation");
        model->select();
    }
    else if(arg1 =="用户关系")
    {
        widget = relation_win;
        widget ->setGeometry(0,20,800,200);
        widget->show();
        model->setTable("relations");
        model->select();
    }
}

void MainWindow::tcp_server()
{
    //连接成功，获取通讯套接字
    msocket = mserver.nextPendingConnection();

    //开始监听套接字消息
    connect(msocket,&QTcpSocket::readyRead,this,&MainWindow::recvData);

    //监听是否退出链接
    connect(msocket,&QTcpSocket::disconnected,this,&MainWindow::disconnect_deal);
}

//接受客户端信息
void MainWindow::recvData()
{
    msocket = dynamic_cast<QTcpSocket *>(MainWindow::sender());

    QByteArray recvbuf = msocket->readAll();
    qDebug()<<recvbuf;

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(recvbuf,&err);
    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"不是Json数据格式";
        return ;
    }
    QJsonObject obj = doc.object();
    if(obj.contains("login"))                                       //登陆操作
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
    else if(obj.contains("sendMessageToFriend"))           //好友消息
    {
        QJsonObject obj1 = obj.value("sendMessageToFriend").toObject();
        QString text = obj1.value("text").toString();
        QString recv_usr = obj1.value("recv_usr").toString();
        QString recv_name = obj1.value("recv_name").toString();
        QString send_usr = obj1.value("send_usr").toString();
        QString send_name = obj1.value("send_name").toString();
        QString time = QDate::currentDate().toString("yyyy-MM-dd");
        time.append(QTime::currentTime().toString(" hh:mm:ss"));
        QSet<usr_info*>::iterator item;
        for (item = tree.begin(); item != tree.end();item++) {
            if((*item)->Usr ==recv_usr)
            {
                //发送好友消息
                QJsonObject obj;
                obj.insert("send_usr",send_usr);
                obj.insert("send_name",send_name);
                obj.insert("type","friend");
                obj.insert("text",text);
                obj.insert("time",time);
                QJsonObject obj_all ;
                obj_all.insert("send_message",obj);
                QJsonDocument doc(obj_all);
                (*item)->msocket->write(doc.toJson());

                //存入记录
                model->setTable("infomation");
                QSqlRecord record = model->record(0);
                record.setValue("send_name",send_name);
                record.setValue("send_usr",send_usr);
                record.setValue("recv_usr",recv_usr);
                record.setValue("text",text);
                record.setValue("recv_name",recv_name);
                record.setValue("time",time);
                record.setValue("type","friend");
                model->insertRecord(0,record);
                break;
            }
        }

        //如果好友不在线，存入未发消息队列
        if(item == tree.end())
        {
            QString table = model->tableName();
            model->setTable("messages");
            QSqlRecord record = model->record(0);
            record.setValue("send_name",send_name);
            record.setValue("send_usr",send_usr);
            record.setValue("recv_usr",recv_usr);
            record.setValue("recv_name",recv_name);
            record.setValue("time",time);
            record.setValue("text",text);
            record.setValue("type","friend");
            model->insertRecord(0,record);
        }
    }
    else if(obj.contains("sendMessageToGroup"))           //群消息
    {
        QJsonObject obj1 = obj.value("sendMessageToGroup").toObject();
        QString recv_name = obj1.value("recv_name").toString();
        QString recv_usr = obj1.value("recv_usr").toString();
        QString send_name = obj1.value("send_name").toString();
        QString send_usr = obj1.value("send_usr").toString();
        QString text = obj1.value("text").toString();
        QString table = model->tableName();
        model->setTable("relations");
        model->setFilter(QString("usr1 = '%1'").arg(recv_usr));
        model->select();
        qDebug()<<recv_usr;
        int n = model->rowCount();
        qDebug()<<"当前有"<<n<<"人";
        for (int i=0 ;i<n;i++)
        {
            QSqlRecord record = model->record(i);
            QString recv_name1 = record.value("name2").toString();
            QString recv_usr1 = record.value("usr2").toString();
            QString type = record.value("type").toString();
            qDebug()<<recv_name1<<" "<<recv_usr1<<" "<<type;
            if(send_usr == recv_usr1)continue;
            QSet<usr_info*>::iterator item ;
            for (item= tree.begin(); item != tree.end(); item++)
            {
                qDebug()<<(*item)->Usr;
                if((*item)->Usr == recv_usr1)                                    //当前在线
                {
                    QString time = QDate::currentDate().toString("yyyy-MM-dd");
                    time.append(QTime::currentTime().toString(" hh:mm:ss"));
                    QJsonObject obj2;
                    obj2.insert("send_name",recv_name);
                    obj2.insert("send_usr",recv_usr);
                    obj2.insert("old_send_name",send_name);
                    obj2.insert("old_send_usr",send_usr);
                    obj2.insert("recv_name",recv_name1);
                    obj2.insert("recv_usr",recv_usr1);
                    obj2.insert("type",type);
                    obj2.insert("text",text);
                    obj2.insert("time",time);
                    QJsonObject all_obj2;
                    all_obj2.insert("send_message",obj2);
                    QJsonDocument doc(all_obj2);
                    (*item)->msocket->write(doc.toJson());
                    model->setTable("infomation");
                    model->setFilter("");
                    model->select();
                    QSqlRecord record = model->record(0);
                    record.setValue("send_name",recv_name);
                    record.setValue("send_usr",recv_usr);
                    record.setValue("recv_name",recv_name1);
                    record.setValue("recv_usr",recv_usr1);
                    record.setValue("type",type);
                    record.setValue("old_send_name",send_name);
                    record.setValue("old_send_usr",send_usr);
                    record.setValue("text",text);
                    record.setValue("time",time);
                    model->insertRecord(0,record);
                    break;
                }
            }
            if(item == tree.end())                                          //当前不在线
            {
                QString time = QDate::currentDate().toString("yyyy-MM-dd");
                time.append(QTime::currentTime().toString(" hh:mm:ss"));

                model->setTable("messages");
                model->setFilter("");
                model->select();
                QSqlRecord record = model->record(0);
                record.setValue("send_name",recv_name);
                record.setValue("send_usr",recv_usr);
                record.setValue("recv_name",recv_name1);
                record.setValue("recv_usr",recv_usr1);
                record.setValue("type",type);
                record.setValue("old_send_name",send_name);
                record.setValue("old_send_usr",send_usr);
                record.setValue("text",text);
                record.setValue("time",time);
                model->insertRecord(0,record);
                model->setTable("relations");
                model->setFilter(QString(QString("usr1 = '%1'").arg(recv_usr)));
                model->select();
            }
        }
        model->setTable(table);
        model->setFilter("");
        model->select();
    }
    else if(obj.contains("switch"))
    {
        //开始发送好友关系数据
        QString usr = obj.value("usr").toString();
        QString tabel = model->tableName();
        model->setTable("relations");
        model->setFilter(QString(" usr1= '%1' or usr2 = '%1'").arg(usr));
        model->select();
        QJsonArray arry;
        qDebug()<<model->rowCount();
        for (int i =0; i<model->rowCount();i++) {
            QSqlRecord record = model->record(i);
            if(record.value(1).toString() == usr)
            {
                QJsonObject obj1;
                obj1.insert("name",record.value(2).toString());
                obj1.insert("usr",record.value(3).toString());
                obj1.insert("type",record.value(4).toString());
                arry.insert(i,obj1);
            }
            else {
                QJsonObject obj1;
                obj1.insert("name",record.value(0).toString());
                obj1.insert("usr",record.value(1).toString());
                obj1.insert("type",record.value(4).toString());
                arry.insert(i,obj1);
            }
            qDebug()<<i;
        }
        QJsonObject send_obj;
        send_obj.insert("relations",arry);
        QJsonDocument doc(send_obj);
        QByteArray send_buf = doc.toJson();
        msocket->write(send_buf);
        model->setTable(tabel);
        model->setFilter("");
        model->select();
    }
    else if(obj.contains("ready_recv"))                 //将离线时消息发送给刚登陆的客户
    {
        qDebug()<<"发送离线消息";
        QString usr  = obj.value("ready_recv").toString();
        QString name = obj.value("name").toString();
        QString table = model->tableName();
        model->setTable("messages");
        model->setFilter(QString("recv_usr = '%1'").arg(usr));
        model->select();
        int n = model->rowCount();
        qDebug()<<n;
        if(n >0)
        {
            QJsonArray arry;
            for (int i= 0; i<n;i++)
            {
                QSqlRecord record = model->record(0);
                QString send_name = record.value("send_name").toString();
                QString send_usr = record.value("send_usr").toString();
                QString text = record.value("text").toString();
                QString time = record.value("time").toString();
                QString type = record.value("type").toString();
                QString old_send_usr;
                QString old_send_name;
                if(type == "group")
                {
                    old_send_name = record.value("old_send_name").toString();
                    old_send_usr = record.value("old_send_usr").toString();
                }

                QJsonObject obj;
                obj.insert("send_name",send_name);
                obj.insert("send_usr",send_usr);
                obj.insert("text",text);
                obj.insert("time",time);
                obj.insert("type",type);
                if(type == "group")
                {
                    obj.insert("old_send_usr",old_send_usr);
                    obj.insert("old_send_name",old_send_name);
                }

                arry.insert(i,obj);
                model->removeRow(0);

                //存入记录
                model->setTable("infomation");
                QSqlRecord record1 = model->record(0);
                record1.setValue("send_name",send_name);
                record1.setValue("send_usr",send_usr);
                record1.setValue("recv_usr",usr);
                record1.setValue("text",text);
                record1.setValue("recv_name",name);
                record1.setValue("time",time);
                record1.setValue("type",type);
                if(type =="group")
                {
                    record1.setValue("old_send_usr",old_send_usr);
                    record1.setValue("old_send_name",old_send_name);
                }
                model->insertRecord(0,record1);
                model->setTable("messages");
                model->setFilter(QString("recv_usr = '%1'").arg(usr));
                model->select();
            }
            QJsonObject obj_all;
            obj_all.insert("out_line_text",arry);
            QJsonDocument doc(obj_all);
            msocket->write(doc.toJson());
        }

        model->setFilter("");
        model->select();
        model->setTable(table);
        model->select();
    }
    else if(obj.contains("add_friend"))                     //添加好友或者群聊
    {
        QJsonObject obj1 = obj.value("add_friend").toObject();
        QString send_name = obj1.value("send_name").toString();
        QString send_usr = obj1.value("send_usr").toString();
        QString recv_usr = obj1.value("recv_usr").toString();
        qDebug()<<recv_usr;
        QString recv_name;
        QString table = model->tableName();
        //判断两人是否已经有好友关系
        model->setTable("relations");
        model->setFilter(QString("usr1 = '%1' and usr2 = '%2'").arg(send_usr).arg(recv_usr));
        model->select();
        //当前已经是好友
        if(model->rowCount()>0)return ;
        model->setFilter(QString("usr1 = '%2' and usr2 = '%1'").arg(send_usr).arg(recv_usr));
        model->select();
        if(model->rowCount()>0)return ;

        model->setTable("user");
        model->setFilter(QString("usr = '%1'").arg(recv_usr));
        model->select();
        int n = model->rowCount();
        if(n> 0)
        {
            QSqlRecord record = model->record(0);
            recv_name = record.value("name").toString();
            qDebug()<<recv_name;
            model->setTable("relations");
            model->setFilter("");
            model->select();
            QSqlRecord record1 = model->record(0);
            record1.setValue("name1",send_name);
            record1.setValue("usr1",send_usr);
            record1.setValue("usr2",recv_usr);
            record1.setValue("name2",recv_name);
            record1.setValue("type","friend");
            model->insertRecord(0,record1);
            QJsonObject obj2;
            obj2.insert("name",recv_name);
            obj2.insert("usr",recv_usr);
            obj2.insert("type","friend");
            QJsonObject obj3;
            obj3.insert("name",send_name);
            obj3.insert("usr",send_usr);
            obj3.insert("type","friend");
            QJsonArray arr;
            arr.insert(0,obj2);
            QJsonArray arr1;
            arr1.insert(0,obj3);
            QJsonObject all_obj;
            all_obj.insert("relations",arr);
            QJsonDocument doc(all_obj);
            QJsonObject all_obj2;
            all_obj2.insert("relations",arr1);
            QJsonDocument doc2(all_obj2);
            //查询对方是否在线
            for (QSet<usr_info*>::iterator item = tree.begin(); item != tree.end(); item++) {
                if((*item)->Usr == recv_usr)
                {
                    (*item)->msocket->write(doc2.toJson());
                    qDebug()<<"对方在线";
                    break;
                }
            }
            msocket->write(doc.toJson());
        }
        else
        {
            //判断是否已加入群聊
            model->setTable("relations");
            model->setFilter(QString("usr1 = '%1' and usr2 = '%2'").arg(recv_usr).arg(send_usr));
            model->select();
            if(model->rowCount()>0)return ;         //当前已经加入群聊，直接返回。

            model->setTable("groups");
            model->setFilter(QString ("group_id = '%1'").arg(recv_usr));
            model->select();
            n = model->rowCount();
            //当前加入的群存在
            if(n> 0 )
            {
                QSqlRecord record = model->record(0);
                int n = record.value("members").toInt();
                n++;
                record.setValue("members",n);
                model->setRecord(0,record);
                recv_name =record.value("group_name").toString();
                model->setTable("relations");
                model->setFilter("");
                model->select();
                QSqlRecord record1 = model->record(0);
                record1.setValue("name1",recv_name);
                record1.setValue("usr1",recv_usr);
                record1.setValue("name2",send_name);
                record1.setValue("usr2",send_usr);
                record1.setValue("type","group");
                model->insertRecord(0,record1);
                QJsonObject obj2;
                obj2.insert("name",recv_name);
                obj2.insert("usr",recv_usr);
                obj2.insert("type","group");
                QJsonArray arr;
                arr.insert(0,obj2);
                QJsonObject all_obj;
                all_obj.insert("relations",arr);
                QJsonDocument doc(all_obj);
                msocket->write(doc.toJson());
            }
            else        //当前群不存在
            {
                QJsonObject obj2;
                obj2.insert("add_failed",recv_usr);
                QJsonDocument doc(obj2);
                msocket->write(doc.toJson());
            }
        }
        model->setTable(table);
        model->setFilter("");
        model->select();
    }
    else if(obj.contains("create_group"))                       //创建群聊
    {
        QJsonObject obj1 = obj.value("create_group").toObject();
        QString group_name = obj1.value("group_name").toString();
        QString group_id = obj1.value("group_id").toString();
        QString send_name = obj1.value("send_name").toString();
        QString send_usr = obj1.value("send_usr").toString();
        QString table = model->tableName();
        model->setTable("groups");
        model->setFilter(QString("group_id = '%1'").arg(group_id));
        model->select();
        int n = model->rowCount();
        if(n<1)
        {
            QSqlRecord record = model->record(0);
            record.setValue("group_name",group_name);
            record.setValue("group_id",group_id);
            record.setValue("members",1);
            model->insertRecord(0,record);
            model->setTable("relations");
            model->setFilter("");
            model->select();
            QSqlRecord record1 = model->record(0);
            record1.setValue("name1",group_name);
            record1.setValue("usr1",group_id);
            record1.setValue("name2",send_name);
            record1.setValue("usr2",send_usr);
            record1.setValue("type","group");
            model->insertRecord(0,record1);
            QJsonObject obj2;
            obj2.insert("name",group_name);
            obj2.insert("usr",group_id);
            obj2.insert("type","group");
            QJsonArray arr;
            arr.insert(0,obj2);
            QJsonObject all_obj;
            all_obj.insert("relations",arr);
            QJsonDocument doc(all_obj);
            msocket->write(doc.toJson());
        }
        model->setTable(table);
        model->setFilter("");
        model->select();
    }
    else if(obj.contains("register_info"))         //注册用户
    {
        QJsonObject obj1 = obj.value("register_info").toObject();
        QString name = obj1.value("name").toString();
        QString usr = obj1.value("usr").toString();
        QString pwd = obj1.value("pwd").toString();
        model->setTable("user");
        model->setFilter(QString("usr = '%1'").arg(usr));
        model->select();
        if(model->rowCount()>0)
        {
            QJsonObject obj2;
            obj2.insert("register_result","no");
            QJsonDocument doc(obj2);
            msocket->write(doc.toJson());
        }
        else
        {
            QSqlRecord record =  model->record(0);
            record.setValue("name",name);
            record.setValue("usr",usr);
            record.setValue("pwd",pwd);
            model->insertRecord(0,record);
            model->setTable("relations");
            model->setFilter("");
            model->select();
            QSqlRecord record1 =  model->record(0);
            record1.setValue("name1","系统聊天室");
            record1.setValue("usr1","00000001");
            record1.setValue("name2",name);
            record1.setValue("usr2",usr);
            record1.setValue("type","group");
            model->insertRecord(0,record1);
            model->setTable("groups");
            model->setFilter(QString("group_id = '00000001'"));
            model->select();
            QSqlRecord record2 =  model->record(0);
            int n = record2.value("members").toInt()+1;
            record2.setValue("members",n);
            model->setRecord(0,record2);
            QJsonObject obj2;
            obj2.insert("register_result","yes");
            QJsonDocument doc(obj2);
            msocket->write(doc.toJson());
        }
    }
}

//客户端断开连接处理
void MainWindow::disconnect_deal()
{
    msocket = dynamic_cast<QTcpSocket *>(MainWindow::sender());

    disconnect(msocket,&QTcpSocket::readyRead,this,&MainWindow::recvData);
    disconnect(msocket,&QTcpSocket::disconnected,this,&MainWindow::disconnect_deal);
    msocket->close();
    usr_info *p;
    for (QSet<usr_info *>::Iterator item = tree.begin() ;item != tree.end();item++) {
        if((*item)->msocket == msocket)
        {
            p=(*item);
            break;
        }
    }
    tree.remove(p);
    qDebug()<<"关闭套接字完成";
}

//添加用户操作
void MainWindow::add_usr(QString name, QString usr, QString pwd)
{
    if(name == "" || usr == "" || pwd == "")
    {
        QMessageBox::warning(this,"操作提示","请输入完整的信息");
        return ;
    }
    QString table = model->tableName();
    model->setTable("user");
    model->setFilter(QString("usr = '%1'").arg(usr));
    model->select();
    if(model->rowCount()>0)
    {
        model->setFilter("");
        model->select();
        QMessageBox::warning(this,"操作提示","账户usr名已被使用！");
        return ;
    }
    QSqlRecord record = model->record(0);
    record.setValue(0,name);
    record.setValue(1,usr);
    record.setValue(2,pwd);

    //添加用户
    model->insertRecord(0,record);
    model->setFilter("");
    model->select();

    //用户加入系统群聊
    model->setTable("relations");
    QSqlRecord record1 =  model->record(0);
    record1.setValue("name1","系统聊天室");
    record1.setValue("usr1","000000001");
    record1.setValue("name2",name);
    record1.setValue("usr2",usr);
    record1.setValue("type","group");
    model->insertRecord(0,record1);

    model->setTable("groups");
    model->setFilter(QString("group_id = '00000001'"));
    model->select();
    QSqlRecord record2 =  model->record(0);
    int n = record2.value("members").toInt()+1;
    record2.setValue("members",n);
    model->setRecord(0,record2);

    model->setTable(table);
    model->setFilter("");
    model->select();
}

//删除用户操作
void MainWindow::delete_usr(QString name, QString usr)
{
    QString table = model->tableName();
    model->setFilter(QString("name = '%1' and usr = '%2'").arg(name).arg(usr));
    model->select();

    if(model->rowCount()<=0)
    {
        model->setFilter("");
        model->select();
        QMessageBox::warning(this,"消息提醒","对不起，未找到该用户！");
        return ;
    }

    //删除用户
    model->removeRow(0);
    model->setFilter("");
    model->select();

    //清除关系
    model->setTable("relations");
    model->setFilter(QString("usr1 = '%1' or usr2 = '%1'").arg(usr));
    model->select();
    int n = model->rowCount();
    qDebug()<<n;
    for (int i = 0;i<n;i++) {
        model->removeRow(i);
    }
    model->setTable(table);
    model->setFilter("");
    model->select();
}

//查询用户操作
void MainWindow::select_usr(QString name, QString usr)
{
    QString sql;
    if(name ==""&&usr =="")
    {
        model->setFilter("");
        model->select();
        return;
    }
    else if(name =="")
    {
        sql = QString("usr like '%%1%'").arg(usr);
    }
    else if(usr =="")
    {
        sql = QString("name like '%%1%'").arg(name);
    }

    model->setFilter(sql);
    model->select();
}

//更新用户操作
void MainWindow::update_usr(QString old, QString name, QString usr, QString pwd)
{
    qDebug()<<"更新数据";
    QString table = model->tableName();
    model->setTable("user");
    model->setFilter(QString("name = '%1' or usr = '%1' ").arg(old));
    model->select();
    if(model->rowCount()<=0)
    {
        QMessageBox::warning(this,"提示消息","对不起，未查找到该用户！");
        model->setTable(table);
        model->setFilter("");
        model->select();
        return;
    }
    //判段账号是否被他人使用
    model->setFilter(QString("name = '%1' or usr = '%1' ").arg(usr));
    model->select();
    if(model->rowCount()>0)
    {
        QMessageBox::warning(this,"提示消息","对不起，该用户账号已被使用！");
        model->setTable(table);
        model->setFilter("");
        model->select();
        return;
    }
    QSqlRecord record = model->record(0);
    record.setValue(0,name);
    record.setValue(1,usr);
    record.setValue(2,pwd);
    model->setRecord(0,record);

    model->setFilter("");
    model->select();
}

void MainWindow::info_select(QString sender, QString recver, QString text)
{
    model->setTable("infomation");
    if(sender ==""&& recver =="" && text == "")
    {
        model->setFilter("");
    }
    else model->setFilter(QString("send_usr = '%1' or send_name = '%1' or recv_name = '%2' or recv_usr = '%2' or text = '%3'").arg(sender).arg(recver).arg(text));
    model->select();
}

void MainWindow::relation_select(QString sender, QString recver)
{
    if(sender =="" && recver =="")model->setFilter("");
    else model->setFilter(QString("usr1 = '%1' or name1 = '%1' or usr2 = '%2' or name2 = '%2' ").arg(sender).arg(recver));
    model->select();
}




