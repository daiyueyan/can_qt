#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ControlCAN.h"
#include "includes.h"
#include <QDebug>
#include "canparamsetting.h"
#include "protocolthrend.h"
#include "canthread.h"
#include "sendthreadcan.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QThread sendThreadCAN;
    QTimer timerCAN;
signals:
    void send_can_out_send(int id, QString ch);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void displayText(QString info);

private:
    Ui::MainWindow *ui;
    CANParamSetting *CANsetting;
    SendThreadCan *thread_cansend;
    CANThread *canthread;
    ProtocolThrend *protocolHand;
    QThread thread;
    void init();
};

#endif // MAINWINDOW_H
