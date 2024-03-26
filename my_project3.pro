#-------------------------------------------------
#
# Project created by QtCreator 2023-10-27T17:40:48
#
#-------------------------------------------------

QT       += core gui sql network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_project3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    my_runing_tasks.cpp \
    user_win.cpp \
    disdeal_message_win.cpp \
    group_win.cpp \
    relations.cpp \
    infomation.cpp \
    usr_info.cpp

HEADERS += \
        mainwindow.h \
    my_runing_tasks.h \
    user_win.h \
    disdeal_message_win.h \
    group_win.h \
    relations.h \
    infomation.h \
    usr_info.h

FORMS += \
        mainwindow.ui \
    user_win.ui \
    disdeal_message_win.ui \
    group_win.ui \
    relations.ui \
    infomation.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
