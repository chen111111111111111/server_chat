#ifndef INFOMATION_H
#define INFOMATION_H

#include <QWidget>

namespace Ui {
class infomation;
}

class infomation : public QWidget
{
    Q_OBJECT

public:
    explicit infomation(QWidget *parent = nullptr);
    ~infomation();
signals:
    void info_select(QString sender,QString recver,QString text);
private slots:
    void on_pushButton_clicked();

private:
    Ui::infomation *ui;
};

#endif // INFOMATION_H
