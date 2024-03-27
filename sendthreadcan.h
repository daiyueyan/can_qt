#ifndef SENDTHREADCAN_H
#define SENDTHREADCAN_H

#include <QThread>
#include "ControlCAN.h"
#include "includes.h"
#include <QDebug>
#include <QString>

class SendThreadCan : public QThread
{
    Q_OBJECT
public:
    SendThreadCan();
    void send_can_out_rev(int id, QString ch);
    void send_can_info_rev(int type, int index, int com, bool open_flag);

private:
    VCI_CAN_OBJ obj;
    int deviceType;
    int debicIndex;
    int debicCom;
    bool open;

    void run();
    void sleep(unsigned int msec);

};

#endif // SENDTHREADCAN_H
