#include "group_win.h"
#include "ui_group_win.h"

group_win::group_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::group_win)
{
    ui->setupUi(this);
}

group_win::~group_win()
{
    delete ui;
}
