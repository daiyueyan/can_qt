#-------------------------------------------------
#
# Project created by QtCreator 2018-08-06T15:00:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CAN
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    canthread.cpp \
    canparamsetting.cpp \
    mymethod.cpp \
    mytimer.cpp \
    protocolthrend.cpp \
    sendthreadcan.cpp \
    upgrade.cpp

HEADERS += \
    includes.h \
        mainwindow.h \
    ControlCAN.h \
    canthread.h \
    canparamsetting.h \
    mymethod.h \
    mytimer.h \
    protocolthrend.h \
    sendthreadcan.h \
    upgrade.h

FORMS += \
        mainwindow.ui \
    canparamsetting.ui \
    upgrade.ui

CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
//LIBS += -L"C:\Qt\build-qwt-Desktop_Qt_5_9_3_MinGW_32bit-Debug\lib" -lqwtd
//LIBS += -L"C:\Qt\build-qwt-Desktop_Qt_5_9_3_MinGW_32bit-Debug\lib" -lqwt
INCLUDEPATH += C:\Qt\Qt5.9.3\5.9.3\mingw53_32\include\qwt

win32: LIBS += -L$$PWD/./ -lControlCAN

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

DISTFILES += \
    Upgrade.qml \
    UpgradeForm.ui.qml

RESOURCES += \
    resonrces.qrc
