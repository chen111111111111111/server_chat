#include "infomation.h"
#include "ui_infomation.h"

infomation::infomation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::infomation)
{
    ui->setupUi(this);
}

infomation::~infomation()
{
    delete ui;
}

void infomation::on_pushButton_clicked()
{
    QString sender = ui->lineEdit->text();
    QString recver = ui->lineEdit_2->text();
    QString text = ui->lineEdit_3->text();

    emit info_select(sender,recver,text);
}
