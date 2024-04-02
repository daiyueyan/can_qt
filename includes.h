#ifndef INCLUDES_H
#define INCLUDES_H

#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMetaType>
#include <QMessageBox>
#include <QRegExp>
#include <QPalette>
#include <QDateTime>
#include "ControlCAN.h"
#include <memory>

#define PRINTF_LOCATION() qDebug()<< "ret in "<< __FILE__ << " at " <<__LINE__

#define RET_VALUE_IF_NOT_EAQU(a,b,c)  \
    do {  \
    if(a!=b) \
{        \
    PRINTF_LOCATION();\
    return c; \
}        \
    } while (false)

#define RET_VALUE_IF_EAQU(a,b,c)  \
    do {  \
    if(a==b) \
{        \
    PRINTF_LOCATION();\
    return c; \
}        \
    } while (false)

#define RET_IF_NOT_EAQU(a,b)  \
    do {  \
    if(a!=b) \
{        \
    PRINTF_LOCATION();\
    return; \
}        \
    } while (false)

#define RET_IF_EAQU(a,b)  \
    do {  \
    if(a==b) \
{        \
    PRINTF_LOCATION();\
    return; \
}        \
    } while (false)

#define Z_OCTAL 8
#define Z_DECIMAL 10
#define Z_HEX 16
#define YMODEM_READ_SEND_OUT 6
#define CAN_FILE_CMD_SIZE 1
#define RT_CAN_YMODEM_DEVICE_SIZE 1100

enum RET_VALUE{
    RET_ERR = 0,
    RET_OK
};

#define OPEN_IF 1
#define CLOSE_IF 0


typedef enum{
    CAN_ID_TOPCTRL_TO_OBOX     = 0x53 ,
    CAN_ID_OBOX_TO_TOPCTRL     = 0x35 ,
    CAN_ID_QT_TO_CTRLBOX = 0x83,
    CAN_ID_CTRLBOX_TO_QT = 0x38,
    CAN_ID_QT_TO_TOPCBOX = 0x85,
    CAN_ID_TOPCBOX_TO_QT = 0x58,
    CAN_ID_OBOX_TO_TOPCTRL_FILE= 0x3A ,
    CAN_ID_TOPCBOX_TO_PC_FILE = 0x4A,
    CAN_ID_QT_TO_OBOX_FILE = 0x8A,
    CAN_ID_QT_TO_TOPCBOX_FILE = 0x81,
}can_id;

typedef enum
{
    CAN_CMD_FILE_CMD_MASTER = 0,                //主�?��??件�?�令
    CAN_CMD_FILE_CMD_SLAVE,                     //�??��??件�?�令
    CAN_CMD_FILE_FILE_MASTER,                   //主�?��??�?
    CAN_CMD_FILE_FILE_SLAVE,                    //�??��??�?
    CAN_CMD_FILE_CTRL_TO_TOPCTRL_FILE_SLAVE,    //主�?��?��??�头?��?��?��????�?
    CAN_CMD_FILE_CTRL_TO_TOPCTRL_FILE_SLAVE_CRC,    //主�?��?��??�头?��?��?��????件�??crc??
    PROTOCOL_CMD_TOPCTRL_TO_CTRLBOX_UPGRADE_PER,
    PROTOCOL_CMD_QT_TO_CTRLBOX_UPGRADE_PER,
    PROTOCOL_CMD_TOPCTRL_TO_CTRLBOX_UPGRADE_END,
    PROTOCOL_CMD_QT_TO_CTRLBOX_UPGRADE_END,
    CAN_CMD_FILE_QT_TO_CTRL_FILE_SLAVE_CRC,
    CAN_CMD_FILE_QT_TO_TOPCTRL_FILE_SLAVE_CRC,
    CAN_CMD_FILE_QT_TO_CTRL_FILE_START,
    CAN_CMD_FILE_QT_TO_TOPCBOX_FILE_START,
    CAN_CMD_FILE_QT_TO_CTRL_FILE_FILE,
    CAN_CMD_FILE_QT_TO_TOPCBOX_FILE_FILE,
    CAN_CMD_FILE_QT_TO_CTRL_FILE_UPGRADE,
    CAN_CMD_FILE_QT_TO_TOPCBOX_FILE_UPGRADE,
    CAN_CMD_FILE_QT_TO_CTRL_FILE_UPGRADE_SUCC,
    CAN_CMD_FILE_QT_TO_TOPCBOX_FILE_UPGRADE_SUCC,

}can_cmd_file_t;

enum CAN_STATUS{
    UNINIT = 0,//?��??�???
    UNCONNECT,//?��???
    OPENDED,//??�?
    INITED,//??�???
    STARTED//就绪
};

/*can????类�??*/
enum CAN_SEND_TYPE{
    CAN_SEND_NORMAL = 0,//正常
    CAN_SEND_SIGNAL,//??�?
    CAN_SEND_SELF,//?��???��??
    CAN_SEND_SELF_SIGNAL//??次�?��???��??
};

/*can?��?�类??*/
enum CAN_DATA_TYPE{
    CAN_DATA_INFO=0,//?��?�帧
    CAN_DATA_REMOTE//�?�?�?
};

enum CAN_EXTERN_TYPE{
    CAN_FRAM_STANDARD=0,//????�?
    CAN_FRAM_EXTERN//?��?�?
};

/*�?�?类�??*/
enum CAN_TRANSMIT_TYPE{
    CAN_SEND=0,//????
    CAN_RCV//?��??
};

/*????帧�??�???�?*/
struct CAN_SEND_FRAME_STRUCT
{
    QString packetTyepStr;
    QString idStr;
    QString dataStr;
    QString sourceStr;
    QString aimStr;
    QString timeStr;
    QString meaningStr;
    bool isSendToSelf;
    int sendCount;
};

/*?��??类�??*/
#define SEND_SELF "??
#define SEND_NORMAL "??


/*?��??类�??*/
enum PACKET_TYPE{
    GATHER_PACKET=1,//信�?��?????��?????��??
    CONTROL_PACKET,//设�??�主?�箱?��?��?��??
    STATE_PACKET,//????模�???��???��??
    TESTING_PACKET,//轨�???��?�箱?��??�?�??��??
    INFO_PACKET,//?��?�信?��?��??
    ALL_PACKET//???????��??
};

/*?��??�?�?*/
enum PACKET_ADDRES{
    CPU_ADDR=1,//主�?��?��?��??
    GATHRE_ADDR,//?????��?��??
    BOTTOM_ADDR,//�?箱�?��?��?��?��??
    SIDE_ADDR,//侧箱?��?��?��?��??
    TOP_ADDR,//车顶?��?��?��?��??
    BROADCAST_ADDR,//广�?��?��??
    ALL_ADDR//?????��??
};

typedef struct rt_can_ymodem_rx_fifo
{
   uint8_t *buffer;

   uint16_t put_index, get_index;

   bool is_full;
}can_ymodem_rx_fifo_t;

struct rt_can_ymodem_device
{
    uint8_t device_type;

    void *can_ymodem_rx;

    uint32_t channel;
};
typedef struct rt_can_ymodem_device rt_can_ymodem_t;

#endif // INCLUDES_H
