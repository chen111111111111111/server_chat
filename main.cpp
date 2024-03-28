#include "mainwindow.h"
#include <QApplication>
#include "appinit.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppInit::Instance()->start();
    MainWindow w;
    w.show();
    return a.exec();
}
