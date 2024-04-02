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
    CAN_CMD_FILE_CMD_MASTER = 0,                //ä¸»æ?ºæ??ä»¶å?½ä»¤
    CAN_CMD_FILE_CMD_SLAVE,                     //ä»??ºæ??ä»¶å?½ä»¤
    CAN_CMD_FILE_FILE_MASTER,                   //ä¸»æ?ºæ??ä»?
    CAN_CMD_FILE_FILE_SLAVE,                    //ä»??ºæ??ä»?
    CAN_CMD_FILE_CTRL_TO_TOPCTRL_FILE_SLAVE,    //ä¸»æ?§æ?¿ç??ºå¤´?§å?¶æ?¿ç????ä»?
    CAN_CMD_FILE_CTRL_TO_TOPCTRL_FILE_SLAVE_CRC,    //ä¸»æ?§æ?¿ç??ºå¤´?§å?¶æ?¿ç????ä»¶ç??crc??
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
    UNINIT = 0,//?ªå??å§???
    UNCONNECT,//?ªè???
    OPENDED,//??å¼?
    INITED,//??å§???
    STARTED//å°±ç»ª
};

/*can????ç±»å??*/
enum CAN_SEND_TYPE{
    CAN_SEND_NORMAL = 0,//æ­£å¸¸
    CAN_SEND_SIGNAL,//??æ¬?
    CAN_SEND_SELF,//?ªå???ªæ??
    CAN_SEND_SELF_SIGNAL//??æ¬¡è?ªå???ªæ??
};

/*can?°æ?®ç±»??*/
enum CAN_DATA_TYPE{
    CAN_DATA_INFO=0,//?°æ?®å¸§
    CAN_DATA_REMOTE//è¿?ç¨?å¸?
};

enum CAN_EXTERN_TYPE{
    CAN_FRAM_STANDARD=0,//????å¸?
    CAN_FRAM_EXTERN//?©å?å¸?
};

/*ä¼?è¾?ç±»å??*/
enum CAN_TRANSMIT_TYPE{
    CAN_SEND=0,//????
    CAN_RCV//?¥æ??
};

/*????å¸§ç??ç»???ä½?*/
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

/*?ªå??ç±»å??*/
#define SEND_SELF "??
#define SEND_NORMAL "??


/*?¥æ??ç±»å??*/
enum PACKET_TYPE{
    GATHER_PACKET=1,//ä¿¡å?·é?????¿é?????¥æ??
    CONTROL_PACKET,//è®¾å??´ä¸»?§ç®±?§å?¶æ?¥æ??
    STATE_PACKET,//????æ¨¡å???¶æ???¥æ??
    TESTING_PACKET,//è½¨æ???§å?¶ç®±?¶æ??æ£?æµ??¥æ??
    INFO_PACKET,//?¿å?¡ä¿¡?¯æ?¥æ??
    ALL_PACKET//???????¥æ??
};

/*?°å??å®?ä¹?*/
enum PACKET_ADDRES{
    CPU_ADDR=1,//ä¸»æ?§æ?¿å?°å??
    GATHRE_ADDR,//?????¿å?°å??
    BOTTOM_ADDR,//æ²?ç®±æ?§å?¶æ?¿å?°å??
    SIDE_ADDR,//ä¾§ç®±?§å?¶æ?¿å?°å??
    TOP_ADDR,//è½¦é¡¶?§å?¶æ?¿å?°å??
    BROADCAST_ADDR,//å¹¿æ?­å?°å??
    ALL_ADDR//?????°å??
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
