#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QThread>
#include "ControlCAN.h"
#include <stdint.h>
#include "includes.h"
#include <QMetaType>
#include <QDebug>
#include <QString>

class CANThread:public QThread
{
    Q_OBJECT
public:
    CANThread();

    void stop();

    bool openCAN();

    void closeCAN();

    int deviceType;
    int debicIndex;
    int baundRate;
    int debicCom;
    int packetType=0;

    bool stopped;

signals:
    //接收到can数据的信号，然后发送给主线程进行显示
    void getProtocolData(QString info);
    void send_can_info_send(int type, int index, int com, bool open_flag);
    void dev_open_send(bool flag);
    void upgrade_info(VCI_CAN_READ can_read_ymodem);
private:
    std::vector<QString> addrsVc;
    std::vector<QString> packeetVc;

    void run();
    void sleep(unsigned int msec);
    void can_process(VCI_CAN_READ can_read);

};

#endif // CANTHREAD_H
