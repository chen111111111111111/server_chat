#ifndef MY_RUNING_TASKS_H
#define MY_RUNING_TASKS_H

#include <QObject>
#include <QRunnable>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
class login_tasks : public QRunnable
{
    Q_OBJECT
public:
    login_tasks(QJsonObject obj);

    void run() override;

private:
    QJsonObject obj;
};

#endif // MY_RUNING_TASKS_H
