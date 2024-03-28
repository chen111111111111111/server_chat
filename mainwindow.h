#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//自定义窗口头文件
#include "user_win.h"
#include "disdeal_message_win.h"
#include "group_win.h"
#include "relations.h"
#include "infomation.h"
#include "usr_info.h"
#include "quiwidget.h"

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

//数据库文件显示
#include <QSqlTableModel>
#include <QSqlRecord>

//服务器套接字头文件
#include <QTcpServer>
#include <QTcpSocket>

//Json数据头文件
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include <QThreadPool>
#include <QFile>

//容器头文件
#include <QSet>

//提示弹窗头文件
#include <QMessageBox>

#include <QTime>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        void my_disconnect(QWidget * w);

    private slots:

        //界面跳转槽函数
    void on_comboBox_currentTextChanged(const QString &arg1);

    //tcp链接槽函数
    void tcp_server();
    void recvData();
    void disconnect_deal();

    //用户管理界面信号处理槽函数
    void add_usr(QString name,QString usr,QString pwd);
    void delete_usr(QString name,QString usr);
    void select_usr(QString name,QString usr);
    void update_usr(QString old,QString name,QString usr,QString pwd);

    void info_select(QString sender,QString recver,QString text);

    void relation_select(QString sender,QString recver);

    //消息处理函数
    void usr_login(QJsonObject obj);
    void MtoFriend(QJsonObject obj);
    void MtoGroup(QJsonObject obj);
    void switch_init(QJsonObject obj);
    void offline_data(QJsonObject obj);
    void add_relation(QJsonObject obj);
    void create_group(QJsonObject obj);
    void register_usr(QJsonObject obj);

    void init_current_style();


    void on_pushButton_closeWin_clicked();

    void on_pushButton_maxWin_clicked();

    void on_pushButton_minWin_clicked();

private:
    Ui::MainWindow *ui;

    QWidget *widget;       //显示自定义窗口
    relations *relation_win;
    user_win *user_w;
    group_win *group_w;
    disDeal_message_win *d_m_w;
    infomation *info_win;

    QThreadPool *global_pool;

    QSqlDatabase db;       //创建数据库
    QTcpServer mserver;    //创建服务器
    QTcpSocket *msocket;   //创建链接套接字

    QSqlTableModel *model; //显示数据库表数据
    QSet<usr_info *> tree;
};

#endif // MAINWINDOW_H
