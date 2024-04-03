#ifndef UPGRADE_H
#define UPGRADE_H

#include <QDialog>
#include <string.h>
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include "sendthreadcan.h"
#include "YmodemFileTransmit.h"
#include "YmodemFileReceive.h"
#include "canthread.h"


namespace Ui {
class Upgrade;
}

class Upgrade : public QDialog
{
    Q_OBJECT

public:
    explicit Upgrade(QWidget *parent = nullptr);
    ~Upgrade();

    void HexString(quint8 vhex,QByteArray &vba);
    void dev_open_get(bool flag);
    void ymodem_can_get(VCI_CAN_READ can_read_ymodem);
    void upgrade_info_get(VCI_CAN_READ can_read_ymodem);
    void can_ymodem_rx(uint8_t *data, uint32_t length, uint32_t *len_return);

    uint32_t upgrade_app_crc_cal(void);
    uint8_t board_name;
    QString file_location;


signals:
    void ymodem_can_read(VCI_CAN_READ can_read_ymodem);
    void ymodem_can_write(VCI_CAN_READ can_write_ymodem);

private:
    int fileSize;
    int pkgsize;        //×Ö½Ú
    int sendedByte;  
    bool transmitButtonStatus;
    bool receiveButtonStatus;
    bool dev_open;
    uint32_t crc_value;

    QFile *sendfile;
    QByteArray txtcon;
    SendThreadCan *thread_cansend;
    YmodemFileTransmit *ymodemFileTransmit;
    YmodemFileReceive* ymodemFileReceive;
    can_ymodem_rx_fifo_t* rx_fifo;

private slots:
    void on_btn_open_file_clicked();

    void on_btn_upgrade_out_clicked();

    void on_board_type_currentIndexChanged(const QString &arg1);

    void on_btn_upgrade_clicked();

    void transmitProgress(int progress);
    void receiveProgress(int progress);
    void transmitStatus(YmodemFileTransmit::Status status);
    void receiveStatus(YmodemFileReceive::Status status);
    void ymodem_can_send(VCI_CAN_READ can_write_ymodem);
    void on_btn_send_clicked();

    void on_btn_revbrowse_clicked();

    void on_btn_rev_clicked();

private:
    Ui::Upgrade *ui;
};

#endif // UPGRADE_H
