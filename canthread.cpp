#include "canthread.h"
#include "ControlCAN.h"
#include <QTime>
#include <QCoreApplication>
#include "mymethod.h"
#include <QMetaType>
#include <string.h>

VCI_BOARD_INFO vbi;

CANThread::CANThread()
{
    stopped = false;
    qRegisterMetaType<VCI_CAN_READ>("VCI_CAN_READ");
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    qRegisterMetaType<DWORD>("DWORD");
}

void CANThread::stop()
{
    stopped = true;
}

bool CANThread::openCAN()
{
    int nDeviceType = deviceType; /* USBCAN-2A或USBCAN-2C或CANalyst-II */
    int nDeviceInd = debicIndex; /* 第1个设备 */
    int nCANInd = debicCom; /* 第1个通道 */
    DWORD dwRel;
    dwRel = VCI_OpenDevice(nDeviceType, nDeviceInd, nCANInd);
    if(dwRel != 1)
    {
        qDebug()<<"open can fail！ ";
        return false;
    }
    else

    dwRel = VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd);
    dwRel = VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd+1);
    VCI_INIT_CONFIG vic;
    vic.AccCode=0x80000008;
    vic.AccMask=0xFFFFFFFF;
    vic.Filter=1;
    vic.Mode=0;
    switch (baundRate) {
    case 10:
        vic.Timing0=0x31;
        vic.Timing1=0x1c;
        break;
    case 20:
        vic.Timing0=0x18;
        vic.Timing1=0x1c;
        break;
    case 40:
        vic.Timing0=0x87;
        vic.Timing1=0xff;
        break;
    case 50:
        vic.Timing0=0x09;
        vic.Timing1=0x1c;
        break;
    case 80:
        vic.Timing0=0x83;
        vic.Timing1=0xff;
        break;
    case 100:
        vic.Timing0=0x04;
        vic.Timing1=0x1c;
        break;
    case 125:
        vic.Timing0=0x03;
        vic.Timing1=0x1c;
        break;
    case 200:
        vic.Timing0=0x81;
        vic.Timing1=0xfa;
        break;
    case 250:
        vic.Timing0=0x01;
        vic.Timing1=0x1c;
        break;
    case 400:
        vic.Timing0=0x80;
        vic.Timing1=0xfa;
        break;
    case 500:
        vic.Timing0=0x00;
        vic.Timing1=0x1c;
        break;
    case 666:
        vic.Timing0=0x80;
        vic.Timing1=0xb6;
        break;
    case 800:
        vic.Timing0=0x00;
        vic.Timing1=0x16;
        break;
    case 1000:
        vic.Timing0=0x00;
        vic.Timing1=0x14;
        break;
    case 33:
        vic.Timing0=0x09;
        vic.Timing1=0x6f;
        break;
    case 66:
        vic.Timing0=0x04;
        vic.Timing1=0x6f;
        break;
    case 83:
        vic.Timing0=0x03;
        vic.Timing1=0x6f;
        break;
    default:
        break;
    }
    dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &vic);
    dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd+1, &vic);
    if(dwRel !=1)
    {
        return false;
    }
    else

    dwRel = VCI_ReadBoardInfo(nDeviceType, nDeviceInd, &vbi);
    if(dwRel != 1)
    {
        return false;
    }
    else
    {
        qDebug()<<"CAN通道数："<<vbi.can_Num;
        qDebug()<<"硬件版本号:"<<vbi.hw_Version;
        qDebug()<<"接口库版本号："<<vbi.in_Version;
        qDebug()<<"中断号"<<vbi.irq_Num;
    }

    if(VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd) !=1)
    {
        return false;
    }
    else
        qDebug()<<"start"<<nCANInd<<"success:";

    int comid = nCANInd+1;
    if(VCI_StartCAN(nDeviceType, nDeviceInd, comid) !=1)
    {
        return false;
    }
    else
        qDebug()<<"start"<<comid<<"success:";

    emit send_can_info_send(deviceType, debicIndex, debicCom, true);
    emit dev_open_send(true);

    return true;
}

void CANThread::closeCAN()
{
    VCI_CloseDevice(deviceType, debicIndex);
    emit send_can_info_send(deviceType, debicIndex, debicCom, false);
    emit dev_open_send(false);
}

void CANThread::run()
{
    DWORD dwRel = 0;
    VCI_CAN_OBJ vco[1024];
    int bufLength=0;
    QString ret;
    QString showInfo;

    while(!stopped)
    {
        sleep(1000);
        bufLength = VCI_GetReceiveNum(deviceType, debicIndex, debicCom);
        if(bufLength != 0)
        {
            dwRel = VCI_Receive(deviceType, debicIndex, debicCom, vco,1024,0);
            if(dwRel > 0)
            {
                for(int i = 0;i<dwRel;i++)
                {
                    VCI_CAN_READ can_read_data;
                    can_read_data.ID = vco[i].ID;
                    can_read_data.Len = vco[i].DataLen;
                    memcpy(can_read_data.Data,vco[i].Data,vco[i].DataLen);
                    //获取ID
                    QString idStr = QString("%1").arg(can_read_data.ID,8,16,QLatin1Char('0'));
                    //idStr就是can发送过来的can的ID的16进制了
                    //获取数据
                    QByteArray dataBa;
                    dataBa.resize(can_read_data.Len);
                    memcpy(dataBa.data(),can_read_data.Data,can_read_data.Len);
                    QString dataStr = dataBa.toHex().toUpper();
                    Mymethod::GetInstance()->addSpaceInQString(dataStr);//为数据之间加上空格
                    //dataStr就是can发送过来的数据格式，只是数据没有包含can_id和帧类型
                    //qDebug()<<"idStr is "<< idStr;

                    can_process(can_read_data);
                    //获取时间
                    QDateTime dt = QDateTime::currentDateTime();
                    QString timeStr = dt.toString("yyyy.MM.dd ");
                    int hour = vco[i].TimeStamp/36000000;
                    int minute = (vco[i].TimeStamp-hour*36000000)/600000;
                    int second = (vco[i].TimeStamp - hour*36000000 - minute*600000)/10000;
                    int ms = (vco[i].TimeStamp - hour*36000000 - minute*600000 - second*10000)/10;
                    QString canTimeStr = QString("%1:").arg(hour,2,10,QChar('0'));
                    canTimeStr += QString("%1:").arg(minute,2,10,QChar('0'));
                    canTimeStr += QString("%1.").arg(second,2,10,QChar('0'));
                    canTimeStr += QString("%1").arg(ms,3,10,QChar('0'));
                    timeStr += canTimeStr;
                    ret =timeStr + "    " +  idStr + "    " + dataStr + "\n";
                    showInfo += ret;
                }
                if(showInfo != "")
                {
                    emit getProtocolData(showInfo.trimmed());
                }
            }
            else if(dwRel == -1)
            {
                qDebug()<<"设备不存在或USB掉线";
            }
        }
    }

    stopped = false;
}

void CANThread::sleep(unsigned int msec)
{
   QTime dieTime = QTime::currentTime().addMSecs(msec);
   while( QTime::currentTime() < dieTime )
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CANThread::can_process(VCI_CAN_READ can_read)
{
    switch (can_read.ID)
    {
    case CAN_ID_CTRLBOX_TO_QT:
        
    break;
        
    case CAN_ID_TOPCBOX_TO_QT:
        
    break;
        
    case CAN_ID_QT_TO_OBOX_FILE:
        emit upgrade_info(can_read);
    break;

    case CAN_ID_QT_TO_TOPCBOX_FILE:

    break;
        
    default:
        
    break;
    }
}
