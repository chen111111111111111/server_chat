#ifndef DISDEAL_MESSAGE_WIN_H
#define DISDEAL_MESSAGE_WIN_H

#include <QWidget>

namespace Ui {
class disDeal_message_win;
}

class disDeal_message_win : public QWidget
{
    Q_OBJECT

public:
    explicit disDeal_message_win(QWidget *parent = nullptr);
    ~disDeal_message_win();

private:
    Ui::disDeal_message_win *ui;
};

#endif // DISDEAL_MESSAGE_WIN_H
