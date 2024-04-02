#include "sendthreadcan.h"
#include <QCoreApplication>
#include "mainwindow.h"
#include "mymethod.h"
#include "includes.h"
#include <string.h>
#include <QTime>

SendThreadCan::SendThreadCan()
{
    open = false;
}

void SendThreadCan::run()
{
    while(1)
    {
//        if(obj.DataLen)
//        {
//            if(1 == VCI_Transmit(deviceType,debicIndex,debicCom,&obj,1))
//            {
//                qDebug()<<"发送成功";
//            }
//        }

        sleep(1000);
    }
}

void SendThreadCan::sleep(unsigned int msec)
{
   QTime dieTime = QTime::currentTime().addMSecs(msec);
   while( QTime::currentTime() < dieTime )
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void SendThreadCan::send_can_info_rev(int type, int index, int com, bool open_flag)
{
    deviceType = type;
    debicIndex = index;
    debicCom = com;

    open = open_flag;
}

int SendThreadCan::can_ymode_send(VCI_CAN_READ can_buff)
{
    if(STATUS_OK == can_send_data(can_buff.ID, can_buff.Data, can_buff.Len))
    {
        return STATUS_OK;
    }
    else
    {
        return STATUS_ERR;
    }
}
//数据帧
int SendThreadCan::can_send_data(uint32_t id, uint8_t *buf, uint8_t len)
{
    VCI_CAN_OBJ obj;

    memset(&obj,0,sizeof(obj));

    obj.ID = id;
    obj.SendType = CAN_SEND_NORMAL;
    obj.RemoteFlag = CAN_DATA_INFO;
    obj.ExternFlag = CAN_FRAM_STANDARD;
    obj.DataLen = len;

    memcpy(obj.Data,buf,obj.DataLen);

    if(1 == VCI_Transmit(deviceType,debicIndex,debicCom,&obj,1))
    {
        return STATUS_OK;
    }
    else
    {
        return STATUS_ERR;
    }
}
//远程帧
void SendThreadCan::can_send_remote(uint32_t id, uint8_t *buf, uint8_t len)
{
    VCI_CAN_OBJ obj;

    memset(&obj,0,sizeof(obj));

    obj.ID = id;
    obj.SendType = CAN_SEND_NORMAL;
    obj.RemoteFlag = CAN_DATA_REMOTE;
    obj.ExternFlag = CAN_FRAM_STANDARD;
    obj.DataLen = len;

    memcpy(obj.Data,buf,obj.DataLen);

    if(1 == VCI_Transmit(deviceType,debicIndex,debicCom,&obj,1))
    {
        qDebug()<<"远程帧发送成功";
    }
}

void SendThreadCan::send_can_out_rev(int id, QString ch)
{
    VCI_CAN_OBJ obj;
    uint8_t buf[1024] = {0};

    memset(&obj,0,sizeof(obj));
    //ID
    obj.ID = id;
    obj.DataLen = ch.remove(QRegExp("\\s")).size()/2;//去掉空格
    //数据
    QByteArray ba;
    QString tmpStr = ch;
    RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
    memcpy(buf, ba.data(),obj.DataLen);
    can_send_data(obj.ID, buf, obj.DataLen);
}

