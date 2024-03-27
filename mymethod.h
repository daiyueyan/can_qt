#ifndef MYMETHOD_H
#define MYMETHOD_H
#include "includes.h"
#include "ControlCAN.h"
class Mymethod
{
private:
    Mymethod();
public:
    static Mymethod *GetInstance();
public:
    int getBytesFromQString(QString& str,QByteArray& ba);//ʮ�������ַ���ת����
    int getTransmitRetQString(int way,const VCI_CAN_OBJ& obj,QString& str);//��objת��Ϊһ�п��ӻ����ַ���
    int addSpaceInQString(QString& str);//ÿ�����ַ���һ���ո�

    int getPacketType(const DWORD& id);//��ȡ��������
    int getSourceAdres(const DWORD& id);//��ȡԴ��ַ
    int getAimAdres(const DWORD& id);//��ȡԴ��ַ
    int getHeart(const DWORD& id);//��ȡ����
};

#endif // MYMETHOD_H
