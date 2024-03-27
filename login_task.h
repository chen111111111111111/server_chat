#ifndef LOGIN_TASK_H
#define LOGIN_TASK_H

#include <QObject>
#include <QRunnable>
class login_task : public QRunnable
{
    Q_OBJECT
public:
    login_task();
    void run() override;
};

#endif // LOGIN_TASK_H
