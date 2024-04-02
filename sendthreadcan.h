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
    int can_send_data(uint32_t id, uint8_t *buf, uint8_t len);
    void can_send_remote(uint32_t id, uint8_t *buf, uint8_t len);
    int can_ymode_send(VCI_CAN_READ can_buff);
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
