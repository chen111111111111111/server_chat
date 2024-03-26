#ifndef GROUP_WIN_H
#define GROUP_WIN_H

#include <QWidget>

namespace Ui {
class group_win;
}

class group_win : public QWidget
{
    Q_OBJECT

public:
    explicit group_win(QWidget *parent = nullptr);
    ~group_win();

private:
    Ui::group_win *ui;
};

#endif // GROUP_WIN_H
