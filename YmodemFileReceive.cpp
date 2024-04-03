#include "YmodemFileReceive.h"

#define READ_TIME_OUT   (10)
#define WRITE_TIME_OUT  (100)

YmodemFileReceive::YmodemFileReceive(QObject *parent) :
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

YmodemFileReceive::~YmodemFileReceive()
{
    delete file;
    delete readTimer;
    delete writeTimer;
}

void YmodemFileReceive::setFilePath(const QString &path)
{
    filePath = path + "/";
}

void YmodemFileReceive::setPortName(const QString &name)
{

}

void YmodemFileReceive::setPortBaudRate(qint32 baudrate)
{

}

bool YmodemFileReceive::startReceive()
{
    progress = 0;
    status   = StatusEstablish;

    readTimer->start(READ_TIME_OUT);

    return true;
}

void YmodemFileReceive::stopReceive()
{
    file->close();
    abort();
    status = StatusAbort;
    writeTimer->start(WRITE_TIME_OUT);
}

int YmodemFileReceive::getReceiveProgress()
{
    return progress;
}

Ymodem::Status YmodemFileReceive::getReceiveStatus()
{
    return status;
}

void YmodemFileReceive::readTimeOut()
{
    readTimer->stop();

    receive();

    if((status == StatusEstablish) || (status == StatusTransmit))
    {
        readTimer->start(READ_TIME_OUT);
    }
}

void YmodemFileReceive::writeTimeOut()
{
    writeTimer->stop();
    receiveStatus(status);
}

Ymodem::Code YmodemFileReceive::callback(Status status, uint8_t *buff, uint32_t *len)
{
    switch(status)
    {
        case StatusEstablish:
        {
            if(buff[0] != 0)
            {
                int  i         =  0;
                char name[128] = {0};
                char size[128] = {0};

                for(int j = 0; buff[i] != 0; i++, j++)
                {
                    name[j] = buff[i];
                }

                i++;

                for(int j = 0; buff[i] != 0; i++, j++)
                {
                    size[j] = buff[i];
                }

                fileName  = QString::fromLocal8Bit(name);
                QString file_desc(size);
                QString sizeStr = file_desc.left(file_desc.indexOf(' '));
                fileSize  = sizeStr.toULongLong();
                fileCount = 0;

                file->setFileName(filePath + fileName);

                if(file->open(QFile::WriteOnly) == true)
                {
                    YmodemFileReceive::status = StatusEstablish;

                    receiveStatus(StatusEstablish);

                    return CodeAck;
                }
                else
                {
                    YmodemFileReceive::status = StatusError;

                    writeTimer->start(WRITE_TIME_OUT);

                    return CodeCan;
                }
            }
            else
            {
                YmodemFileReceive::status = StatusError;

                writeTimer->start(WRITE_TIME_OUT);

                return CodeCan;
            }
        }

        case StatusTransmit:
        {
            if((fileSize - fileCount) > *len)
            {
                file->write((char *)buff, *len);

                fileCount += *len;
            }
            else
            {
                file->write((char *)buff, fileSize - fileCount);

                fileCount += fileSize - fileCount;
            }

            progress = (int)(fileCount * 100 / fileSize);

            YmodemFileReceive::status = StatusTransmit;

            receiveProgress(progress);
            receiveStatus(StatusTransmit);

            return CodeAck;
        }

        case StatusFinish:
        {
            file->close();

            YmodemFileReceive::status = StatusFinish;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeAck;
        }

        case StatusAbort:
        {
            file->close();

            YmodemFileReceive::status = StatusAbort;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }

        case StatusTimeout:
        {
            YmodemFileReceive::status = StatusTimeout;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }

        default:
        {
            file->close();

            YmodemFileReceive::status = StatusError;

            writeTimer->start(WRITE_TIME_OUT);

            return CodeCan;
        }
    }
}

uint32_t YmodemFileReceive::read(uint8_t *buff, uint32_t len)
{
    uint32_t len_re = 0;
    uint32_t* len_return = new uint32_t(0);

    emit transmit_read_request(buff, len, len_return);
    len_re = *len_return;
    delete len_return;
    len_return = nullptr;

    return len_re;
}

uint32_t YmodemFileReceive::write(uint8_t *buff, uint32_t len)
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
