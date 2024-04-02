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
    CodeSoh  = 0x01,//SOH表示128字节数据包
    CodeStx  = 0x02,//STX表示1024字节数据包
    CodeEot  = 0x04,//EOT表示结束传输，由发送端发送
    CodeAck  = 0x06,//ACK表示回应
    CodeNak  = 0x15,//NAK表示不回应
    CodeCan  = 0x18,//CA表示传输中止，由发送端发送
    CodeC    = 0x43,//C表示请求数据包
    CodeA1   = 0x41,
    CodeA2   = 0x61
  };

  enum Stage
  {
    StageNone,//还没开始的阶段
    StageEstablishing,//建立阶段
    StageEstablished,//已经建立阶段
    StageTransmitting,//传输阶段
    StageFinishing,//完成阶段
    StageFinished//已经建立阶段
  };

  enum Status
  {
    StatusEstablish,//状态建立
    StatusTransmit,//状态传输
    StatusFinish,//状态中止
    StatusAbort,//状态中止
    StatusTimeout,//状态超时
    StatusError//状态错误
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
