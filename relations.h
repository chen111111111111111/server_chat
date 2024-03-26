#ifndef RELATIONS_H
#define RELATIONS_H

#include <QWidget>

namespace Ui {
class relations;
}

class relations : public QWidget
{
    Q_OBJECT

public:
    explicit relations(QWidget *parent = nullptr);
    ~relations();
signals:
    void relation_select(QString sender,QString recver);

private slots:
    void on_pushButton_clicked();

private:
    Ui::relations *ui;
};

#endif // RELATIONS_H
