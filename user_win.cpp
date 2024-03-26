#include "user_win.h"
#include "ui_user_win.h"

user_win::user_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_win)
{
    ui->setupUi(this);
}

user_win::~user_win()
{
    delete ui;
}

void user_win::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString usr = ui->lineEdit_2->text();
    QString pwd = ui->lineEdit_3->text();

    emit add_send(name,usr,pwd);
}

void user_win::on_pushButton_2_clicked()
{
    QString name = ui->lineEdit_4->text();
    QString usr = ui->lineEdit_5->text();
    emit select_send(name,usr);
}

void user_win::on_pushButton_3_clicked()
{
    QString old = ui->lineEdit_6->text();
    QString name = ui->lineEdit_7->text();
    QString usr = ui->lineEdit_8->text();
    QString pwd = ui->lineEdit_9->text();
    emit update_send(old,name,usr,pwd);
}

void user_win::on_pushButton_4_clicked()
{
    QString name = ui->lineEdit_10->text();
    QString usr = ui->lineEdit_11->text();
    emit delete_send(name,usr);
}
