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

void SendThreadCan::send_can_out_rev(int id, QString ch)
{
    memset(&obj,0,sizeof(obj));
    //ID
    obj.ID = id;
    //发送类型:正常
    obj.SendType = CAN_SEND_NORMAL;
    //数据类型:数据帧
    obj.RemoteFlag = CAN_DATA_INFO;
    //是否扩展帧：标准帧
    obj.ExternFlag = CAN_FRAM_STANDARD;
    //数据长度
    obj.DataLen = ch.remove(QRegExp("\\s")).size()/2;//去掉空格
    //数据
    QByteArray ba;
    QString tmpStr = ch;
    RET_IF_NOT_EAQU(Mymethod::GetInstance()->getBytesFromQString(tmpStr,ba),RET_OK);
    memcpy(obj.Data,ba.data(),obj.DataLen);
    if(1 == VCI_Transmit(deviceType,debicIndex,debicCom,&obj,1))
    {
        qDebug()<<"发送成功";
    }
}
