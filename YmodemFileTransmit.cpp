#include "YmodemFileTransmit.h"
#include <QFileInfo>
#include "includes.h"

#define READ_TIME_OUT   (10)
#define WRITE_TIME_OUT  (100)

YmodemFileTransmit::YmodemFileTransmit(QObject *parent) :
    QObject(parent),
    file(new QFile),
    readTimer(new QTimer),
    writeTimer(new QTimer)
{
    setTimeDivide(499);
    setTimeMax(5);
    setErrorMax(999);

    connect(readTimer, SIGNAL(timeout()), this, SLOT(readTimeOut()));
    connect(writeTimer, SIGNAL(timeout()), this, SLOT(writeTimeOut()));
}

YmodemFileTransmit::~YmodemFileTransmit()
{
    delete file;
    delete readTimer;
    delete writeTimer;
}

void YmodemFileTransmit::setFileName(const QString &name)
{
    file->setFileName(name);
}

void YmodemFileTransmit::setPortName(const QString &name)
{
    //serialPort->setPortName(name);
}

void YmodemFileTransmit::setPortBaudRate(qint32 baudrate)
{
    //serialPort->setBaudRate(baudrate);
}

bool YmodemFileTransmit::startTransmit()
{
    progress = 0;
    status   = StatusEstablish;

    readTimer->start(READ_TIME_OUT);

    return true;
}

void YmodemFileTransmit::stopTransmit()
{
    file->close();
    abort();
    status = StatusAbort;
    writeTimer->start(WRITE_TIME_OUT);
}

int YmodemFileTransmit::getTransmitProgress()
{
    return progress;
}

Ymodem::Status YmodemFileTransmit::getTransmitStatus()
{
    return status;
}

void YmodemFileTransmit::readTimeOut()
{
    readTimer->stop();

    transmit();

    if((status == StatusEstablish) || (status == StatusTransmit))
    {
        readTimer->start(READ_TIME_OUT);
    }
}

void YmodemFileTransmit::writeTimeOut()
{
    writeTimer->stop();
    transmitStatus(status);
}

Ymodem::Code YmodemFileTransmit::callback(Status status, uint8_t *buff, uint32_t *len)
{
    switch(status)
    {
        case StatusEstablish://自定义的函数_rym_send_begin
        {
            if(file->open(QFile::ReadOnly) == true)//打开文件，
            {
                QFileInfo fileInfo(*file);

                fileSize  = fileInfo.size();//文件大小
                fileCount = 0;
                //把文件的信息弄到buf上
                strcpy((char *)buff, fileInfo.fileName().toLocal8Bit().data());
                strcpy((char *)buff + fileInfo.fileName().toLocal8Bit().size() + 1, QByteArray::number(fileInfo.size()).data());

                *len = YMODEM_PACKET_SIZE;

                YmodemFileTransmit::status = StatusEstablish;

                transmitStatus(StatusEstablish);

                return CodeAck;
            }
            else
            {
                YmodemFileTransmit::status = StatusError;

                writeTimer->start(WRITE_TIME_OUT);

                return CodeCan;
            }
        }

        case StatusTransmit://_rym_send_data函数
        {
            if(fileSize != fileCount)
            {
                if((fileSize - fileCount) > YMODEM_PACKET_SIZE)
                {
                    fileCount += file->read((char *)buff, YMODEM_PACKET_1K_SIZE);

                    *len = YMODEM_PACKET_1K_SIZE;
                }
                else
                {
                    fileCount += file->read((char *)buff, YMODEM_PACKET_SIZE);

                    *len = YMODEM_PACKET_SIZE;
                }

                progress = (int)(fileCount * 100 / fileSize);

                YmodemFileTransmit::status = StatusTransmit;

                transmitProgress(progress);
                transmitStatus(StatusTransmit);

                return CodeAck;
            }
            else
            {
                YmodemFileTransmit::status = StatusTransmit;

                transmitStatus(StatusTransmit);

                return CodeEot;
            }
        }

        case StatusFinish:
        {
            file->close();

            YmodemFileTransmit::status = StatusFinish;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeAck;
        }

        case StatusAbort:
        {
            file->close();

            YmodemFileTransmit::status = StatusAbort;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }

        case StatusTimeout:
        {
            YmodemFileTransmit::status = StatusTimeout;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }

        default:
        {
            file->close();

            YmodemFileTransmit::status = StatusError;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }
    }
}

uint32_t YmodemFileTransmit::read(uint8_t *buff, uint32_t len)
{
    uint32_t len_re = 0;
    uint32_t* len_return = new uint32_t(0);

    emit transmit_read_request(buff, len, len_return);
    len_re = *len_return;
    delete len_return;
    len_return = nullptr;

    return len_re;
}

uint32_t YmodemFileTransmit::write(uint8_t *buff, uint32_t len)
{
    VCI_CAN_READ can_write_ymodem;
    uint8_t size_else = 0;
    uint32_t cur_addr = 0;

    can_write_ymodem.ID = CAN_ID_QT_TO_OBOX_FILE;
    can_write_ymodem.Data[0] = CAN_CMD_FILE_QT_TO_CTRL_FILE_FILE;
    //以7个字节读取数据写入can设备中，因为还有一个功能码
    size_else = len % YMODEM_READ_SEND_OUT;
    for(cur_addr = 0; cur_addr < (len - size_else); cur_addr+=YMODEM_READ_SEND_OUT)
    {
        can_write_ymodem.Len = CAN_FILE_CMD_SIZE + YMODEM_READ_SEND_OUT;
        memcpy(&can_write_ymodem.Data[1], &buff[cur_addr], YMODEM_READ_SEND_OUT);
        emit send_file(can_write_ymodem);
    }
    if(size_else != 0)
    {
        cur_addr = len - size_else;
        can_write_ymodem.Len = size_else + CAN_FILE_CMD_SIZE;
        memcpy(&can_write_ymodem.Data[1], &buff[cur_addr], size_else);
        emit send_file(can_write_ymodem);
    }
    return len;
}
