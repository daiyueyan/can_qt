#include "mytimer.h"

bool auto_add_id=false;

MyTimer::MyTimer(bool isSendOneTime) :
    QThread()
{
    this->isSendOneTime = isSendOneTime;
}
/***********************************************/
// z ��������:NULL
// h ��������:NULL
// u ��������:NULL
// x ��������ֵ:NULL
// y ��ע:NULL
/***********************************************/
int MyTimer::doSendTimer(const VCI_CAN_OBJ &info, const int time)
{
    int ret = RET_OK;
    if(time <= 0)
    {
        ret = RET_ERR;
    }
    this->obj = info;
    this->time = time;

    return ret;
}

/**
 *������:���õ��η��͵�����
 *��������:NULL
 *��������:NULL
 *��������ֵ:NULL
 *��ע:NULL
 */
int MyTimer::setSingalSendData(const std::vector<VCI_CAN_OBJ> &vcCan, std::vector<int> &msleeps)
{
    int ret = RET_OK;
    if(vcCan.size()!=(msleeps.size()))
    {
        ret = RET_ERR;
    }
    else
    {
        this->vcCan = vcCan;
        this->msleeps = msleeps;
    }
    return ret;
}

/***********************************************/
// z ��������:�̺߳���
// h ��������:NULL
// u ��������:NULL
// x ��������ֵ:NULL
// y ��ע:NULL
/***********************************************/
void MyTimer::run()
{
    if(!isSendOneTime)//ѭ������
    {
        if(this->time<=0)
        {
            stop==false;
        }
        unsigned char tmpHeart=0;
        while(!stop)
        {
            if(auto_add_id)//�Ƿ���������
            {
                tmpHeart = (unsigned char)Mymethod::GetInstance()->getHeart(obj.ID);
                tmpHeart++;
                obj.ID = (obj.ID & 0xFFFFFF00) | tmpHeart;
            }
            emit canSendSignal(this->obj);
            this->msleep(this->time);
        }
    }
    else//���ʷ���
    {
        if(vcCan.size()>0)
        {
            int count = vcCan.size();
            int i=0;
            while(count > i)
            {
                VCI_CAN_OBJ tmpObj = vcCan[i];
                emit canSendSignal(tmpObj);
                this->msleep(msleeps[i]);
                i++;
            }
        }
        isSendOneTime = false;
        std::vector<VCI_CAN_OBJ> vcTemp1;vcTemp1.swap(vcCan);
        std::vector<int> vcTemp2;vcTemp2.swap(msleeps);
    }

}
