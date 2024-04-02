#ifndef __YMODEM_H
#define __YMODEM_H

/* Header includes -----------------------------------------------------------*/
#include <stdint.h>

/* Macro definitions ---------------------------------------------------------*/
#define YMODEM_PACKET_HEADER    (3)
#define YMODEM_PACKET_TRAILER   (2)
#define YMODEM_PACKET_OVERHEAD  (YMODEM_PACKET_HEADER + YMODEM_PACKET_TRAILER)
#define YMODEM_PACKET_SIZE      (128)
#define YMODEM_PACKET_1K_SIZE   (1024)

#define YMODEM_CODE_CAN_NUMBER  (5)

/* Type definitions ----------------------------------------------------------*/
class Ymodem
{
public:
  enum Code
  {
    CodeNone = 0x00,
    CodeSoh  = 0x01,//SOH��ʾ128�ֽ����ݰ�
    CodeStx  = 0x02,//STX��ʾ1024�ֽ����ݰ�
    CodeEot  = 0x04,//EOT��ʾ�������䣬�ɷ��Ͷ˷���
    CodeAck  = 0x06,//ACK��ʾ��Ӧ
    CodeNak  = 0x15,//NAK��ʾ����Ӧ
    CodeCan  = 0x18,//CA��ʾ������ֹ���ɷ��Ͷ˷���
    CodeC    = 0x43,//C��ʾ�������ݰ�
    CodeA1   = 0x41,
    CodeA2   = 0x61
  };

  enum Stage
  {
    StageNone,//��û��ʼ�Ľ׶�
    StageEstablishing,//�����׶�
    StageEstablished,//�Ѿ������׶�
    StageTransmitting,//����׶�
    StageFinishing,//��ɽ׶�
    StageFinished//�Ѿ������׶�
  };

  enum Status
  {
    StatusEstablish,//״̬����
    StatusTransmit,//״̬����
    StatusFinish,//״̬��ֹ
    StatusAbort,//״̬��ֹ
    StatusTimeout,//״̬��ʱ
    StatusError//״̬����
  };

  Ymodem(uint32_t timeDivide = 499, uint32_t timeMax = 5, uint32_t errorMax = 999);

  void setTimeDivide(uint32_t timeDivide);
  uint32_t getTimeDivide();

  void setTimeMax(uint32_t timeMax);
  uint32_t getTimeMax();

  void setErrorMax(uint32_t errorMax);
  uint32_t getErrorMax();

  void receive();
  void transmit();
  void abort();

private:
  Code receivePacket();

  void receiveStageNone();
  void receiveStageEstablishing();
  void receiveStageEstablished();
  void receiveStageTransmitting();
  void receiveStageFinishing();
  void receiveStageFinished();

  void transmitStageNone();
  void transmitStageEstablishing();
  void transmitStageEstablished();
  void transmitStageTransmitting();
  void transmitStageFinishing();
  void transmitStageFinished();

  uint16_t crc16(uint8_t *buff, uint32_t len);

  virtual Code callback(Status status, uint8_t *buff, uint32_t *len) = 0;

  virtual uint32_t read(uint8_t *buff, uint32_t len)  = 0;
  virtual uint32_t write(uint8_t *buff, uint32_t len) = 0;

  uint32_t timeDivide;
  uint32_t timeMax;
  uint32_t errorMax;

  uint32_t timeCount;
  uint32_t errorCount;
  uint8_t  dataCount;

  Code  code;
  Stage stage;

  uint8_t  rxBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD];
  uint8_t  txBuffer[YMODEM_PACKET_1K_SIZE + YMODEM_PACKET_OVERHEAD];
  uint32_t rxLength;
  uint32_t txLength;
};

/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

#endif /* __YMODEM_H */
