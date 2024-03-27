#include "mymethod.h"

Mymethod::Mymethod()
{

}

/**
 *������:���ص��ж���
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
Mymethod *Mymethod::GetInstance()
{
    static Mymethod gp;
    return &gp;
}

/**
 *������:NULL
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int Mymethod::getBytesFromQString(QString &str, QByteArray &ba)
{
    int ret = RET_OK;
    str.remove(QRegExp("\\s"));//ȥ���ո�
    if(0 != str.size()%2)//�������ż�����ַ������0
    {
        str = "0"+str;
    }
    ba.resize(str.size()/2);
    QString tmpStr;
    int j=0;
    for(int i=0;i<str.size();i++)
    {
        tmpStr = str[i++];
        tmpStr += str[i];
        ba[j++] = tmpStr.toUInt(nullptr,Z_HEX);
    }
    return ret;
}

/**
 *������:���ط��ͻ��߽��յ���obj�ṹ���ַ���
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int Mymethod::getTransmitRetQString(int way, const VCI_CAN_OBJ &obj, QString &str)
{
    int ret = RET_OK;
    str.clear();
    if(CAN_SEND == way)
    {
        QDateTime dt = QDateTime::currentDateTime();
        QString timeStr = dt.toString("yyyy.MM.dd hh:mm:ss.zzz");
        QString idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        addSpaceInQString(dataStr);
        str = timeStr + "    " + idStr + "    " + dataStr;
    }
    else if(CAN_RCV == way)
    {
        QDateTime dt = QDateTime::currentDateTime();
        QString timeStr = dt.toString("yyyy.MM.dd ");
        int hour = obj.TimeStamp/36000000;
        int minute = (obj.TimeStamp-hour*36000000)/600000;
        int second = (obj.TimeStamp - hour*36000000 - minute*600000)/10000;
        int ms = (obj.TimeStamp - hour*36000000 - minute*600000 - second*10000)/10;
        QString canTimeStr = QString("%1:").arg(hour,2,10,QChar('0'));
        canTimeStr += QString("%1:").arg(minute,2,10,QChar('0'));
        canTimeStr += QString("%1.").arg(second,2,10,QChar('0'));
        canTimeStr += QString("%1").arg(ms,3,10,QChar('0'));
        timeStr += canTimeStr;
        QString idStr = QString("%1").arg(obj.ID,8,16,QLatin1Char('0'));
        QByteArray dataBa;dataBa.resize(obj.DataLen);memcpy(dataBa.data(),obj.Data,obj.DataLen);
        QString dataStr = dataBa.toHex().toUpper();
        addSpaceInQString(dataStr);
        str = timeStr + "    " + idStr + "    " + dataStr;
    }
    else
    {
        ret = RET_ERR;
    }

    return ret;
}

/**
 *������:ÿ�����ַ���һ���ո�
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int Mymethod::addSpaceInQString(QString &str)
{
    int ret = RET_OK;
    QString retStr;
    for(int i=0;i<str.size();i++)
    {
        retStr += str.mid(i,2);
        retStr += " ";
        i++;
    }
    str = retStr;
    return ret;
}

/**
 *������:��ȡ��������
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int Mymethod::getPacketType(const DWORD &id)
{
    int ret = 0;
    ret = (id>>24)&0x1F;


    return ret;
}

/***********************************************/
// z ��������:����can id��ȡԴ��ַ
// h ��������:NULL
// u ��������:NULL
// x ��������ֵ:NULL
// y ��ע:NULL
/***********************************************/
int Mymethod::getSourceAdres(const DWORD &id)
{
    int ret = 0;
    ret = (id>>8)&0x1F;
    return ret;

}

/***********************************************/
// z ��������:����canid��ȡĿ���ַ
// h ��������:NULL
// u ��������:NULL
// x ��������ֵ:NULL
// y ��ע:NULL
/***********************************************/
int Mymethod::getAimAdres(const DWORD &id)
{
    int ret = 0;
    ret = (id>>16)&0x1F;
    return ret;
}

/**
 *������:����canid��ȡ����
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int Mymethod::getHeart(const DWORD &id)
{
    int ret = 0;
    ret = (id>>0)&0xFF;
    return ret;
}
