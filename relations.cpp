#include "relations.h"
#include "ui_relations.h"

relations::relations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::relations)
{
    ui->setupUi(this);
}

relations::~relations()
{
    delete ui;
}

void relations::on_pushButton_clicked()
{
    QString sender = ui->lineEdit->text();
    QString recver = ui->lineEdit_2->text();
    emit relation_select(sender,recver);
}
