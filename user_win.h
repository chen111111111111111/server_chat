#ifndef USER_WIN_H
#define USER_WIN_H

#include <QWidget>

namespace Ui {
class user_win;
}

class user_win : public QWidget
{
    Q_OBJECT

public:
    explicit user_win(QWidget *parent = nullptr);
    ~user_win();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

signals:
    void add_send(QString name,QString usr,QString pwd);
    void select_send(QString name,QString usr);
    void update_send(QString old,QString name,QString usr,QString pwd);
    void delete_send(QString name,QString usr);
private:
    Ui::user_win *ui;
};

#endif // USER_WIN_H
