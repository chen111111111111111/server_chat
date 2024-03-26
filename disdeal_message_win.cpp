#include "disdeal_message_win.h"
#include "ui_disdeal_message_win.h"

disDeal_message_win::disDeal_message_win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disDeal_message_win)
{
    ui->setupUi(this);
}

disDeal_message_win::~disDeal_message_win()
{
    delete ui;
}
